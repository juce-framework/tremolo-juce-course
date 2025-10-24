#pragma once

namespace tremolo {
class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);
  ~PluginEditor() override;

  void resized() override;

private:
  juce::ImageComponent background;
  juce::ImageComponent logo;

  juce::Label waveformLabel{"waveform label", "WAVEFORM"};
  juce::ComboBox waveformComboBox;
  juce::ComboBoxParameterAttachment waveformAttachment;

  juce::Label rateLabel{"rate label", "RATE"};
  juce::Slider rateSlider;
  juce::SliderParameterAttachment rateAttachment;

  juce::Label bypassLabel{"bypass label", "BYPASS"};
  juce::ToggleButton bypassButton{"BYPASSED"};
  juce::ButtonParameterAttachment bypassAttachment;

  LfoVisualizer lfoVisualizer;
  MessageOnClick about;

  CustomLookAndFeel lookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace tremolo
