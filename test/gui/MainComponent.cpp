#include "MainComponent.h"
#define PI2 6.28318530718

extern std::shared_ptr<node> head = nullptr;

TEST(test_xy_ticks) {
  std::vector<float> y_test_data(10);
  std::iota(y_test_data.begin(), y_test_data.end(), 1.f);

  std::vector<float> x_ticks = {0, 1, 7};
  std::vector<float> y_ticks = {3, 2, 9};
  PLOT_Y({y_test_data});
  X_TICKS(x_ticks);
  Y_TICKS(y_ticks);
}

TEST(test_custom_x_labels) {
  std::vector<float> y_test_data(10000);
  std::iota(y_test_data.begin(), y_test_data.end(), -100000.f);
  const std::vector<std::string> labels = {"MMM", "Two", "Three", "Fyra",
                                           "Fem", "Sex", "Sju",   "Atta",
                                           "Nio", "Tio", "Elva",  "Tolv"};
  PLOT_Y({y_test_data});
  X_LABELS(labels)
  X_LABEL("X LABEL");
  Y_LABEL("Y LABEL");
  TITLE("TITLE");
}

TEST(test_custom_y_labels) {
  std::vector<float> y_test_data(10000);
  std::iota(y_test_data.begin(), y_test_data.end(), -100000.f);
  const std::vector<std::string> labels = {"Ett", "Two", "Three", "Fyra",
                                           "Fem", "Sex", "Sju",   "Atta",
                                           "Nio", "Tio", "Elva",  "Tolv"};
  PLOT_Y({y_test_data});
  Y_LABELS(labels)
  X_LABEL("X LABEL");
  Y_LABEL("Y LABEL");
  TITLE("TITLE");
}

TEST(test_semi_plot_x_1000) {
  std::vector<float> y_test_data(1000);
  std::iota(y_test_data.begin(), y_test_data.end(), 1.f);
  SEMI_PLOT_Y({y_test_data});
  GRID_ON;
}

TEST(test_grid_on) {
  std::vector<float> y_test_data(10);
  std::iota(y_test_data.begin(), y_test_data.end(), 0);
  PLOT_Y({y_test_data});
  GRID_ON;
}

TEST(test_tiny_grid_on) {
  std::vector<float> y_test_data(10);
  std::iota(y_test_data.begin(), y_test_data.end(), 0);
  PLOT_Y({y_test_data});
  TINY_GRID_ON;
}

TEST(test_semi_plot_x_tiny_grid_on) {
    std::vector<float> y_test_data(1000);
    std::iota(y_test_data.begin(), y_test_data.end(), 1.f);
    SEMI_PLOT_Y({ y_test_data });
    TINY_GRID_ON;
};

TEST(test_draw_flat_line) {
  std::vector<float> y_data{1, 1};
  std::vector<float> x_data{0, 9};

  PLOT_XY({x_data}, {y_data});
  Y_LIM(0, 2);
}

TEST(test_linear_dashed_lines) {
  std::vector<float> y_test_data(10000);
  std::iota(y_test_data.begin(), y_test_data.end(), -100000.f);
  const std::vector<float> dashed_lengths = {4, 8};
  PLOT_Y({y_test_data});
  MAKE_GRAPH_DASHED(dashed_lengths, 0);
}

TEST(test_flat_curve_10000) {
  std::vector<float> y_test_data(100000);
  std::iota(y_test_data.begin(), y_test_data.end(), -100000.f);
  PLOT_Y({y_test_data});
}

TEST(test_flat_curve_0p0001) {
  std::vector<float> y_test_data(100);
  std::iota(y_test_data.begin(), y_test_data.end(), 0.f);
  for (auto &val : y_test_data) val *= 0.00001;
  PLOT_Y({y_test_data});
}

TEST(test_labels) {
  std::vector<float> y_test_data(10000);
  std::iota(y_test_data.begin(), y_test_data.end(), -100000.f);
  PLOT_Y({y_test_data});
  X_LABEL("X LABEL");
  Y_LABEL("Y LABEL");
  TITLE("TITLE");
}

TEST(test_sinus_auto_lim) {
  std::vector<float> test_data = std::vector<float>(100);
  std::iota(test_data.begin(), test_data.end(), 0.f);
  for (auto &y : test_data) {
    y = std::sin(y * PI2 / test_data.size());
  }
  PLOT_Y({test_data});
}

TEST(test_ramp) {
  std::vector<float> y_test_data(10);
  std::iota(y_test_data.begin(), y_test_data.end(), 0);
  PLOT_Y({y_test_data});
}

TEST(test_two_sine) {
  std::vector<std::vector<float>> test_data_y =
      std::vector<std::vector<float>>(2, std::vector<float>(100));
  float i = 0;
  for (auto &y_vec : test_data_y) {
    std::iota(y_vec.begin(), y_vec.end(), 0);
    for (auto &y : y_vec) {
      y = std::sin(y * PI2 / y_vec.size()) + i;
    }
    i++;
  }

  std::vector<std::vector<float>> test_data_x =
      std::vector<std::vector<float>>(2, std::vector<float>(100));
  std::iota(test_data_x.front().begin(), test_data_x.front().end(), 0);
  std::iota(test_data_x.back().begin(), test_data_x.back().end(), -50);

  PLOT_XY(test_data_x, test_data_y);
}

TEST(test_x_lim) {
  std::vector<float> test_data_y = std::vector<float>(100);
  std::iota(test_data_y.begin(), test_data_y.end(), 0.f);
  for (auto &y : test_data_y) {
    y = std::sin(y * PI2 / test_data_y.size());
  }

  std::vector<float> test_data_x = std::vector<float>(100);
  std::iota(test_data_x.begin(), test_data_x.end(), -49);

  PLOT_XY({test_data_x}, {test_data_y});
  X_LIM(0, 50);
}

TEST(test_y_lim) {
  std::vector<float> test_data_y = std::vector<float>(100);
  std::iota(test_data_y.begin(), test_data_y.end(), 0.f);
  for (auto &y : test_data_y) {
    y = std::sin(y * PI2 / test_data_y.size());
  }

  PLOT_Y({test_data_y});
  Y_LIM(0, 1);
}

static juce::Rectangle<int> getScreenArea() {
  return juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
}

static Plot *getPlotFromID(
    std::map<std::string, std::unique_ptr<Plot>> &plot_holder, const int id) {
  auto it = plot_holder.begin();

  // id != 0
  for (int i = 1; i < plot_holder.size() + 1; ++i) {
    if (id == i) break;
    it++;
  }
  return it->second.get();
}

MainComponent::MainComponent() : m_menu_label("", "Tests: ") {
  setSize(1200, 800);
  ADD_PARENT_COMP(this);

  addAndMakeVisible(m_test_menu);
  addAndMakeVisible(m_menu_label);

  auto it = m_plot_holder.begin();
  for (auto i = 0u; i < m_plot_holder.size(); ++i) {
    m_test_menu.addItem((*it++).first, i + 1);
  }

  for (auto &plot : m_plot_holder) {
    plot.second->setBounds(0, getScreenArea().getHeight() / 15, getWidth(),
                           getHeight() - getScreenArea().getHeight() / 15);
    plot.second->setVisible(false);
  }

  m_test_menu.onChange = [this]() {
    if (m_current_plot != nullptr) {
      m_current_plot->setVisible(false);
    }
    const auto id = m_test_menu.getSelectedId();
    if (!m_plot_holder.empty()) {
      m_current_plot = getPlotFromID(m_plot_holder, id);
      m_current_plot->setVisible(true);
    }
    resized();
  };
}

void MainComponent::paint(juce::Graphics &g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {
  m_test_menu.setBounds(0, getScreenArea().getHeight() / 30, getWidth() / 2,
                        getScreenArea().getHeight() / 30);
  m_menu_label.setBounds(0, 0, getWidth() / 2,
                         getScreenArea().getHeight() / 30);

  for (auto &plot : m_plot_holder) {
    if (plot.second->isVisible()) {
      plot.second->setBounds(0, getScreenArea().getHeight() / 15, getWidth(),
                             getHeight() - getScreenArea().getHeight() / 15);
    }
  }
}
