namespace tremolo {
namespace {
constexpr auto buttonInsetWidth = 2.f;

void drawGradientButton(juce::Graphics& g,
                        const juce::Rectangle<float>& bounds,
                        const juce::ColourGradient& gradient) {
  g.setGradientFill(gradient);
  g.fillRoundedRectangle(bounds, 4.f);
}

void drawBlueGradientButton(juce::Graphics& g,
                            const juce::Rectangle<float>& bounds) {
  auto buttonGradient = juce::ColourGradient::vertical(
      juce::Colour{0xFF4A7090}, juce::Colour{0xFF324258}, bounds);
  buttonGradient.addColour(0.73, juce::Colour{0xFF315160});
  drawGradientButton(g, bounds, buttonGradient);
}

void drawOrangeGradientButton(juce::Graphics& g,
                              const juce::Rectangle<float>& bounds) {
  const auto buttonGradient = juce::ColourGradient::vertical(
      juce::Colour{0xFFFF901A}, juce::Colour{0xFFFFC300}, bounds);
  drawGradientButton(g, bounds, buttonGradient);
}

void drawButtonInset(juce::Graphics& g, const juce::Rectangle<float>& bounds) {
  auto insetGradient = juce::ColourGradient::vertical(
      juce::Colour{0xFF22232C}, juce::Colour{0xFF263235}, bounds);
  insetGradient.addColour(0.35, juce::Colour{0xFF303538});
  g.setGradientFill(insetGradient);
  g.fillRoundedRectangle(bounds, 6.f);
}
}  // namespace

juce::Colour CustomLookAndFeel::getColor(Colors colorName) {
  static const std::array colors{
      juce::Colour{0xFFFFAA00},
      juce::Colour{0xFFDDECFF},
  };
  return colors.at(juce::toUnderlyingType(colorName));
}

CustomLookAndFeel::CustomLookAndFeel() {
  setColour(juce::ComboBox::textColourId, getColor(Colors::paleBlue));
  setColour(juce::Label::textColourId, getColor(Colors::paleBlue));
  const juce::Colour darkBlue{0xFF153245};
  setColour(juce::PopupMenu::backgroundColourId, darkBlue);
  setColour(juce::PopupMenu::textColourId, getColor(Colors::paleBlue));
  setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour{0xFF0C131E});
  setColour(juce::PopupMenu::highlightedBackgroundColourId,
            getColor(Colors::orange));
  setColour(juce::BubbleComponent::backgroundColourId, darkBlue);
  setColour(juce::BubbleComponent::outlineColourId, juce::Colour{0xFF0C0E16});

  // used to set the font of the default standalone plugin window
  getDefaultLookAndFeel().setDefaultSansSerifTypeface(
      interMedium().getTypeface());
}

juce::FontOptions CustomLookAndFeel::getSideLabelsFont() {
  return interMedium().withPointHeight(10.f);
}

juce::BorderSize<int> CustomLookAndFeel::getLabelBorderSize(juce::Label&) {
  return juce::BorderSize{0};
}

juce::FontOptions CustomLookAndFeel::getRateLabelFont() {
  return interBold().withPointHeight(12.f);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                         int x,
                                         int y,
                                         int width,
                                         int height,
                                         float sliderPos,
                                         const float rotaryStartAngle,
                                         const float rotaryEndAngle,
                                         juce::Slider&) {
  const auto bounds = juce::Rectangle{x, y, width, height};
  const auto knobCanalBounds = bounds.toFloat().reduced(3.75f);

  g.setColour(juce::Colour{0xFF2A3A3B});
  g.fillEllipse(knobCanalBounds);

  const auto valueArcBounds = knobCanalBounds.reduced(0.25f);

  juce::Path arc;
  const auto toAngle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  arc.addPieSegment(valueArcBounds, rotaryStartAngle, toAngle, 0.f);
  g.setColour(getColor(Colors::orange));
  g.fillPath(arc);

  const auto knobBounds = knobCanalBounds.reduced(4.f);

  auto knobFill = juce::ColourGradient::vertical(
      juce::Colour{0xFF4A7090}, juce::Colour{0xFF060F1C}, knobBounds);
  knobFill.addColour(0.29, juce::Colour{0xFF396086});
  knobFill.addColour(0.75, juce::Colour{0xFF2C3648});
  g.setGradientFill(knobFill);
  g.fillEllipse(knobBounds);

  // Knob stroke
  g.setColour(juce::Colour{0x400B1E3A});
  constexpr auto knobStrokeThickness = 1.33f;
  g.drawEllipse(knobBounds.reduced(knobStrokeThickness / 2.f),
                knobStrokeThickness);

  // Knob top
  const auto knobTopBounds = knobBounds.reduced(7.f);
  auto knobTopFill = juce::ColourGradient{juce::Colour{0xFF6697CB},
                                          knobTopBounds.getCentreX(),
                                          knobTopBounds.getY() - 7.f,
                                          juce::Colour{0xFF1B1E48},
                                          knobTopBounds.getCentreX(),
                                          knobTopBounds.getBottom() + 41.f,
                                          true};
  knobTopFill.addColour(0.66, juce::Colour{0xFF0C2338});
  g.setGradientFill(knobTopFill);
  g.fillEllipse(knobTopBounds);

  // Knob top edge
  auto knobTopEdgeFill = juce::ColourGradient{juce::Colour{0xFF8FFFFF},
                                              knobTopBounds.getCentreX(),
                                              knobTopBounds.getY(),
                                              juce::Colour{0xFF1A0F4E},
                                              knobTopBounds.getCentreX(),
                                              knobTopBounds.getBottom() + 6.f,
                                              true};
  knobTopEdgeFill.addColour(0.55, juce::Colour{0xFF8078F4});
  g.setGradientFill(knobTopEdgeFill);
  g.setOpacity(0.1f);
  g.drawEllipse(knobTopBounds, 1.f);
}

void CustomLookAndFeel::drawComboBox(juce::Graphics& g,
                                     int width,
                                     int height,
                                     bool /* isButtonDown */,
                                     int /* buttonX */,
                                     int /* buttonY */,
                                     int /* buttonW */,
                                     int /* buttonH */,
                                     juce::ComboBox&) {
  const auto boxBounds = juce::Rectangle{0, 0, width, height}.toFloat();

  drawButtonInset(g, boxBounds);

  const auto buttonBounds = boxBounds.reduced(buttonInsetWidth);
  drawBlueGradientButton(g, buttonBounds);

  auto arrowBounds = boxBounds.reduced(10.f, 11.f);
  arrowBounds.removeFromLeft(104);
  juce::Path arrow;
  arrow.startNewSubPath(arrowBounds.getTopLeft());
  arrow.lineTo(arrowBounds.getCentreX(), arrowBounds.getBottom());
  arrow.lineTo(arrowBounds.getTopRight());

  g.setColour(getColor(Colors::paleBlue));
  g.fillPath(arrow);
}

juce::Font CustomLookAndFeel::getComboBoxFont(juce::ComboBox&) {
  return interMedium().withPointHeight(12.f);
}

void CustomLookAndFeel::positionComboBoxText(juce::ComboBox& comboBox,
                                             juce::Label& labelToPosition) {
  auto bounds = comboBox.getLocalBounds().reduced(10, 6);
  bounds.removeFromRight(12);
  labelToPosition.setBounds(bounds);
  labelToPosition.setJustificationType(juce::Justification::centred);
  labelToPosition.setFont(getComboBoxFont(comboBox));
}

juce::PopupMenu::Options CustomLookAndFeel::getOptionsForComboBoxPopupMenu(
    juce::ComboBox& box,
    juce::Label& label) {
  const auto menuBounds = box.getScreenBounds().reduced(2, 0);
  return juce::LookAndFeel_V4::getOptionsForComboBoxPopupMenu(box, label)
      .withStandardItemHeight(24)
      .withTargetScreenArea(menuBounds)
      .withMinimumWidth(128);
}

juce::Font CustomLookAndFeel::getPopupMenuFont() {
  return interMedium().withPointHeight(12.f);
}

juce::Path CustomLookAndFeel::getTickShape(float) {
  return {};
}

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g,
                                         juce::ToggleButton& button,
                                         bool shouldDrawButtonAsHighlighted,
                                         bool shouldDrawButtonAsDown) {
  juce::ignoreUnused(shouldDrawButtonAsDown, shouldDrawButtonAsHighlighted);

  const auto bounds = button.getLocalBounds().toFloat();

  drawButtonInset(g, bounds);
  const auto buttonBounds = bounds.reduced(buttonInsetWidth);

  if (!button.getToggleState()) {
    drawBlueGradientButton(g, buttonBounds);
    g.setColour(getColor(Colors::paleBlue));
    g.setFont(interMedium().withPointHeight(12.f));
  } else {
    drawOrangeGradientButton(g, buttonBounds);

    const juce::Colour textColour{0xFF501A0B};
    g.setColour(textColour);
    g.setFont(interBold().withPointHeight(12.f));
  }
  g.drawText(button.getButtonText(), bounds, juce::Justification::centred,
             false);
}

juce::FontOptions CustomLookAndFeel::interMedium() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(
      assets::InterMedium_ttf, assets::InterMedium_ttfSize);
  return juce::FontOptions{result};
}

juce::FontOptions CustomLookAndFeel::interBold() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(
      assets::InterBold_ttf, assets::InterBold_ttfSize);
  return juce::FontOptions{result};
}
}  // namespace tremolo
