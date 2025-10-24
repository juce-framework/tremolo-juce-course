namespace tremolo {
PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p),
      waveformAttachment{p.getParameterRefs().waveform, waveformComboBox},
      rateAttachment{p.getParameterRefs().rate, rateSlider},
      bypassAttachment{p.getParameterRefs().bypassed, bypassButton},
      lfoVisualizer{
          [&p](juce::AudioBuffer<float>& b) { p.readAllLfoSamples(b); },
          [&p] { return p.getSampleRateThreadSafe(); },
          [&p] { return p.getParameterRefs().bypassed.get(); }},
      about{*this, logo,
            JucePlugin_Manufacturer "\n" JucePlugin_Name "\n" __DATE__
                                    "\n" __TIME__
                                    "\nv" JucePlugin_VersionString} {
  background.setImage(juce::ImageCache::getFromMemory(
      assets::Background_png, assets::Background_pngSize));
  addAndMakeVisible(background);

  logo.setImage(
      juce::ImageCache::getFromMemory(assets::Logo_png, assets::Logo_pngSize));
  addAndMakeVisible(logo);

  const auto sideFontColor = juce::Colour{0xFF6EA0C7};

  waveformLabel.setJustificationType(juce::Justification::left);
  waveformLabel.setMinimumHorizontalScale(1.f);
  waveformLabel.setFont(lookAndFeel.getSideLabelsFont());
  waveformLabel.setColour(juce::Label::textColourId, sideFontColor);
  addAndMakeVisible(waveformLabel);

  waveformComboBox.addItemList(p.getParameterRefs().waveform.choices, 1);
  waveformAttachment.sendInitialUpdate();
  addAndMakeVisible(waveformComboBox);

  rateSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  rateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                             true, 0, 0);
  rateSlider.setTextValueSuffix(" Hz");
  rateSlider.setPopupDisplayEnabled(true, true, this);
  addAndMakeVisible(rateSlider);

  rateLabel.setJustificationType(juce::Justification::centred);
  rateLabel.setInterceptsMouseClicks(false, false);
  rateLabel.setFont(lookAndFeel.getRateLabelFont());
  addAndMakeVisible(rateLabel);

  bypassLabel.setJustificationType(juce::Justification::left);
  bypassLabel.setMinimumHorizontalScale(1.f);
  bypassLabel.setFont(lookAndFeel.getSideLabelsFont());
  bypassLabel.setColour(juce::Label::textColourId, sideFontColor);
  addAndMakeVisible(bypassLabel);

  bypassButton.onClick = [this]() {
    bypassButton.setButtonText(bypassButton.getToggleState() ? "Bypassed"
                                                             : "Off");
  };
  bypassButton.onClick();
  addAndMakeVisible(bypassButton);

  lfoVisualizer.setCurveWidth(2.f);
  lfoVisualizer.setCurveColor(
      lookAndFeel.getColor(CustomLookAndFeel::Colors::orange));
  lfoVisualizer.setBackgroundColor(juce::Colours::transparentBlack);
  addAndMakeVisible(lfoVisualizer);

  setLookAndFeel(&lookAndFeel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(540, 270);
}

PluginEditor::~PluginEditor() {
  setLookAndFeel(nullptr);
}

void PluginEditor::resized() {
  const auto bounds = getLocalBounds();

  background.setBounds(bounds);

  logo.setBounds({16, 16, 105, 24});

  auto lfoVisualizerBounds = bounds.reduced(18, 27);
  lfoVisualizerBounds.removeFromTop(122);
  lfoVisualizer.setBounds(lfoVisualizerBounds);

  auto rateSliderBounds = bounds.reduced(230, 40);
  rateSliderBounds.removeFromBottom(110);
  rateSlider.setBounds(rateSliderBounds);
  rateLabel.setBounds(rateSliderBounds);

  auto waveformComboBoxBounds = bounds;
  waveformComboBoxBounds.removeFromTop(66);
  waveformComboBoxBounds.removeFromRight(392);
  waveformComboBoxBounds.removeFromBottom(176);
  waveformComboBoxBounds.removeFromLeft(16);
  waveformComboBox.setBounds(waveformComboBoxBounds);

  auto waveformLabelBounds = bounds;
  waveformLabelBounds.removeFromTop(48);

  // we make more space here than in Figma to avoid ellipsis insertion
  waveformLabelBounds.removeFromRight(461);

  waveformLabelBounds.removeFromBottom(206);
  waveformLabelBounds.removeFromLeft(20);

  waveformLabel.setBounds(waveformLabelBounds);

  auto bypassButtonBounds = bounds;
  bypassButtonBounds.removeFromTop(66);
  bypassButtonBounds.removeFromRight(16);
  bypassButtonBounds.removeFromBottom(176);
  bypassButtonBounds.removeFromLeft(392);
  bypassButton.setBounds(bypassButtonBounds);

  auto bypassLabelBounds = bounds;
  bypassLabelBounds.removeFromTop(48);

  // we make more space here than in Figma to avoid ellipsis insertion
  bypassLabelBounds.removeFromRight(104);

  bypassLabelBounds.removeFromBottom(206);
  bypassLabelBounds.removeFromLeft(396);

  bypassLabel.setBounds(bypassLabelBounds);
}
}  // namespace tremolo
