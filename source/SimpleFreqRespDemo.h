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

#include <cmp_plot.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>

using namespace juce;

//==============================================================================
class SimpleFreqRespDemo : public AudioAppComponent, private Timer {
 public:
  SimpleFreqRespDemo() {

    RuntimePermissions::request(
        RuntimePermissions::recordAudio, [this](bool granted) {
          int numInputChannels = granted ? 2 : 0;
          setAudioChannels(numInputChannels, num_channels);
        });

    startTimerHz(60);
    setSize(700, 500);

    addAndMakeVisible(m_plot);

    m_plot.setBounds(getBounds());
    m_plot.yLim(-50.0f, 20.0f);
    m_plot.xLim(100.0f, 18'000.0f);
  }

  ~SimpleFreqRespDemo() override { shutdownAudio(); }

  //==============================================================================
  void prepareToPlay(int /*samplesPerBlockExpected*/,
                     double new_sample_rate) override {
    for (auto& x : x_data) {
      cmp::iota_delta<float>(x.begin(), x.end(), 1.0f,
                             float(new_sample_rate / float(2 * fftSize)));
    }
  }

  void releaseResources() override {
    // (nothing to do here)
  }

  void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
    if (bufferToFill.buffer->getNumChannels() > 0) {
      const auto* channelData =
          bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

      for (auto ch_idx = 0u; ch_idx != num_channels; ++ch_idx) {
        for (auto i = 0; i < bufferToFill.numSamples; ++i)
          pushNextSampleIntoFifo(channelData[i], ch_idx);
      }

      bufferToFill.clearActiveBufferRegion();
    }
  }

  //==============================================================================
  void paint(Graphics& g) override {}

  void timerCallback() override {
    if (nextFFTBlockReady) {
      for (size_t i = 0; i < num_channels; i++) {
        drawNextFrequencyResponse(i);
      }

      nextFFTBlockReady = false;
    }
  }

  void pushNextSampleIntoFifo(const float sample,
                              const std::size_t ch_idx) noexcept {
    if (fifoIndex[ch_idx] == fftSize) {
      if (!nextFFTBlockReady) {
        zeromem(fftData[ch_idx].data(), fftData[ch_idx].size());
        memcpy(fftData[ch_idx].data(), fifo[ch_idx].data(),
               fifo[ch_idx].size());

        nextFFTBlockReady = true;
      }

      fifoIndex[ch_idx] = 0;
    }

    fifo[ch_idx][fifoIndex[ch_idx]++] = sample;
  }

  void drawNextFrequencyResponse(const std::size_t ch_idx) {
    forwardFFT[ch_idx].performFrequencyOnlyForwardTransform(
        fftData[ch_idx].data());

    constexpr auto scale = 1.0f / float(fftSize);

    for (auto& s : fftData[ch_idx]) {
      s = s * scale;

      if (s < 1e-5f) {
        s = -50;
        continue;
      }

      s = 10.0f * log10f(s);
    }

    m_plot.plot(fftData, x_data);
  }

  const enum { fftOrder = 11, fftSize = 1 << fftOrder };

 private:
  static constexpr int num_channels{1};
  
  std::vector<std::vector<float>> x_data = std::vector<std::vector<float>>(
      num_channels, std::vector<float>(2 * fftSize));

  std::array<dsp::FFT, num_channels> forwardFFT = {dsp::FFT(fftOrder)};

  std::vector<std::vector<float>> fifo = std::vector<std::vector<float>>(
      num_channels, std::vector<float>(fftSize));

  std::vector<std::vector<float>> fftData = std::vector<std::vector<float>>(
      num_channels, std::vector<float>(2 * fftSize));

  std::vector<int> fifoIndex = std::vector<int>(num_channels);

  bool nextFFTBlockReady = false;

  cmp::SemiLogX m_plot;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleFreqRespDemo)
};