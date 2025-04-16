#include "Tremolo/JsonSerializer.h"
#include <juce_core/juce_core.h>
#include "Tremolo/Parameters.h"

namespace {
constexpr auto pluginNameId = "pluginName";
constexpr auto modulationRateHzId = "modulationRateHz";
constexpr auto bypassedId = "bypassed";
constexpr auto modulationWaveformId = "modulationWaveform";
}  // namespace

template <class AudioParameter>
struct AudioParameterSerialisationTraits {
  static constexpr auto marshallingVersion = std::nullopt;

  template <typename Archive, typename T>
  static void save(Archive& archive, const T& t) {
    using namespace juce;

    // TODO: Extract length to a named constant (check if necessary)
    archive(named(t.getName(40).toStdString(), t.get()));
  }

  template <typename Archive, typename T>
  static void load(Archive& archive, T& t) {
    using namespace juce;

    auto value = t.get();
    archive(named(t.getName(40).toStdString(), value));
    t = value;
  }
};

template <>
struct juce::SerialisationTraits<juce::AudioParameterFloat>
    : public AudioParameterSerialisationTraits<juce::AudioParameterFloat> {};

template <>
struct juce::SerialisationTraits<juce::AudioParameterBool>
    : public AudioParameterSerialisationTraits<juce::AudioParameterBool> {};

template <>
struct juce::SerialisationTraits<juce::AudioParameterChoice> {
  static constexpr auto marshallingVersion = std::nullopt;

  template <typename Archive, typename T>
  static void save(Archive& archive, const T& t) {
    using namespace juce;

    archive(named(t.getName(40).toStdString(), t.getCurrentChoiceName()));
  }

  template <typename Archive, typename T>
  static void load(Archive& archive, T& t) {
    using namespace juce;

    auto value = t.getCurrentChoiceName();
    archive(named(t.getName(40).toStdString(), value));

    if (const auto index = t.choices.indexOf(value); 0 <= index) {
      t = index;
    }
  }
};

template <>
struct juce::SerialisationTraits<ws::Parameters> {
  static constexpr auto marshallingVersion = 1;

  template <typename Archive, typename T>
  static void serialise(Archive& archive, T& p) {
    using namespace juce;

    if (archive.getVersion() != 1) {
      return;
    }

    std::string pluginName = JucePlugin_Name;

    archive(named(pluginNameId, pluginName));

    if (pluginName != JucePlugin_Name) {
      return;
    }

    archive(named(modulationRateHzId, p.rate), named(bypassedId, p.bypassed),
            named(modulationWaveformId, p.waveform));
  }
};

namespace ws {
void JsonSerializer::serialize(const Parameters& parameters,
                               juce::OutputStream& output) {
  const auto json = juce::ToVar::convert(parameters);

  if (!json.has_value()) {
    return;
  }

  jassert(json->isObject());

  json->getDynamicObject()->writeAsJSON(
      output, juce::JSON::FormatOptions{}
                  .withSpacing(juce::JSON::Spacing::multiLine)
                  .withMaxDecimalPlaces(2));
}

void JsonSerializer::deserialize(juce::InputStream& input,
                                 Parameters& parameters) {
  juce::var parsedResult;
  const auto parsingResult =
      juce::JSON::parse(input.readEntireStreamAsString(), parsedResult);

  if (parsingResult.failed()) {
    DBG(parsingResult.getErrorMessage());
    return;
  }

  const auto parsedParameters =
      juce::FromVar::convert<Parameters>(parsedResult);

  if (!parsedParameters.has_value()) {
    return;
  }

  parameters.rate = parsedParameters->rate.get();
  parameters.bypassed = parsedParameters->bypassed.get();
  parameters.waveform = parsedParameters->waveform.getIndex();
}
}  // namespace ws
