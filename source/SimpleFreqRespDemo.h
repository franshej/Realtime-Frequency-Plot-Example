/*
  ==============================================================================
   This file is a modified SimpleFFTDemo which is a part of the JUCE examples.
   Copyright (c) 2020 - Raw Material Software Limited
   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.
   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.
  ==============================================================================
*/

#pragma once

//==============================================================================
class SimpleFreqRespDemo : public AudioAppComponent, private Timer {
 public:
  SimpleFreqRespDemo()
      : forwardFFT(fftOrder), spectrogramImage(Image::RGB, 512, 512, true) {
    setOpaque(true);

    RuntimePermissions::request(RuntimePermissions::recordAudio,
                                [this](bool granted) {
                                  int numInputChannels = granted ? 2 : 0;
                                  setAudioChannels(numInputChannels, 2);
                                });

    startTimerHz(60);
    setSize(700, 500);
  }

  ~SimpleFreqRespDemo() override { shutdownAudio(); }

  //==============================================================================
  void prepareToPlay(int /*samplesPerBlockExpected*/,
                     double /*newSampleRate*/) override {
    // (nothing to do here)
  }

  void releaseResources() override {
    // (nothing to do here)
  }

  void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
    if (bufferToFill.buffer->getNumChannels() > 0) {
      const auto* channelData =
          bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

      for (auto i = 0; i < bufferToFill.numSamples; ++i)
        pushNextSampleIntoFifo(channelData[i]);

      bufferToFill.clearActiveBufferRegion();
    }
  }

  //==============================================================================
  void paint(Graphics& g) override {
    g.fillAll(Colours::black);

    g.setOpacity(1.0f);
    g.drawImage(spectrogramImage, getLocalBounds().toFloat());
  }

  void timerCallback() override {
    if (nextFFTBlockReady) {
      drawNextLineOfSpectrogram();
      nextFFTBlockReady = false;
      repaint();
    }
  }

  void pushNextSampleIntoFifo(float sample) noexcept {
    // if the fifo contains enough data, set a flag to say
    // that the next line should now be rendered..
    if (fifoIndex == fftSize) {
      if (!nextFFTBlockReady) {
        zeromem(fftData, sizeof(fftData));
        memcpy(fftData, fifo, sizeof(fifo));
        nextFFTBlockReady = true;
      }

      fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
  }

  void drawNextLineOfSpectrogram() {
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // first, shuffle our image leftwards by 1 pixel..
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);

    // find the range of values produced, so we can scale our rendering to
    // show up the detail clearly
    auto maxLevel = FloatVectorOperations::findMinAndMax(fftData, fftSize / 2);

    for (auto y = 1; y < imageHeight; ++y) {
      auto skewedProportionY =
          1.0f - std::exp(std::log((float)y / (float)imageHeight) * 0.2f);
      auto fftDataIndex =
          jlimit(0, fftSize / 2, (int)(skewedProportionY * (int)fftSize / 2));
      auto level = jmap(fftData[fftDataIndex], 0.0f,
                        jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

      spectrogramImage.setPixelAt(rightHandEdge, y,
                                  Colour::fromHSV(level, 1.0f, level, 1.0f));
    }
  }

  enum { fftOrder = 10, fftSize = 1 << fftOrder };

 private:
  dsp::FFT forwardFFT;
  Image spectrogramImage;

  float fifo[fftSize];
  float fftData[2 * fftSize];
  int fifoIndex = 0;
  bool nextFFTBlockReady = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleFreqRespDemo)
};