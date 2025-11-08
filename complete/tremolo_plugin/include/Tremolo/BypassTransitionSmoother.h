#pragma once

namespace tremolo {
/**
 * Class facilitating transition to and from bypassed state over a single block.
 *
 * It allows 2 things:
 *  - detecting that a bypass state transition takes place
 *  - fade-in and fade-out of dry (unprocessed) and wet (processed) buffers
 *    according to the transition type
 *
 * Use the following code in the processBlock() method of your plugin processor:
 *
 * @code
 * //...
 *
 * bypassTransitionSmoother.setBypass(parameters.bypassed);
 *
 * if (bypassTransitionSmoother.isTransitioning()) {
 *   bypassTransitionSmoother.setDryBuffer(buffer);
 *
 *   yourAudioEffectClassInstance.process(buffer);
 *
 *   bypassTransitionSmoother.mixToWetBuffer(buffer);
 *
 *   return;
 * }
 *
 * // avoid processing if bypassed, process otherwise...
 * @endcode
 *
 * Alternatively, you can unconditionally call setDryBuffer() and
 * mixToWetBuffer(), like this:
 *
 * @code
 * //...
 *
 * bypassTransitionSmoother.setBypass(parameters.bypassed);
 *
 * if (parameters.bypassed && !bypassTransitionSmoother.isTransitioning()) {
 *   // avoid processing if the plugin is bypassed
 *   return;
 * }
 *
 * bypassTransitionSmoother.setDryBuffer(buffer);
 * yourAudioEffectClassInstance.process(buffer);
 * bypassTransitionSmoother.mixToWetBuffer(buffer);
 *
 * // no more processing necessary
 * @endcode
 *
 * Remember to call prepare() in prepareToPlay(),
 * setBypassForced() in setStateInformation(), and reset() in
 * releaseResources().
 */
class BypassTransitionSmoother {
public:
  explicit BypassTransitionSmoother(double crossfadeLengthSecondsValue = 0.01)
      : crossfadeLengthSeconds{crossfadeLengthSecondsValue} {
    jassert(0.0 < crossfadeLengthSeconds);

    reset();
  }

  void prepare(const juce::dsp::ProcessSpec& spec) {
    sampleRateHz = spec.sampleRate;
    dryBuffer.setSize(static_cast<int>(spec.numChannels),
                      static_cast<int>(spec.maximumBlockSize));
    dryGain.reset(spec.sampleRate, crossfadeLengthSeconds);
    wetGain.reset(spec.sampleRate, crossfadeLengthSeconds);
    reset();
  }

  void setBypass(bool bypass) noexcept {
    if (bypass == isBypassed()) {
      return;
    }

    const auto current = dryGain.getCurrentValue();
    const auto target = bypass ? 1.0f : 0.0f;
    const auto duration = crossfadeLengthSeconds * std::abs(target - current);

    dryGain.reset(sampleRateHz, duration);
    wetGain.reset(sampleRateHz, duration);

    dryGain.setCurrentAndTargetValue(current);
    dryGain.setTargetValue(target);

    wetGain.setCurrentAndTargetValue(1.0f - current);
    wetGain.setTargetValue(1.0f - target);
  }

  void setBypassForced(bool bypass) noexcept {
    dryGain.setCurrentAndTargetValue(bypass ? 1.0f : 0.0f);
    wetGain.setCurrentAndTargetValue(1.0f - dryGain.getTargetValue());
  }

  void setDryBuffer(const juce::AudioBuffer<float>& buffer) noexcept {
    jassert(buffer.getNumSamples() <= dryBuffer.getNumSamples());
    jassert(buffer.getNumChannels() <= dryBuffer.getNumChannels());

    for (const auto channel : std::views::iota(0, buffer.getNumChannels())) {
      dryBuffer.copyFrom(channel, 0, buffer, channel, 0,
                         buffer.getNumSamples());
    }
    dryGain.applyGain(dryBuffer, buffer.getNumSamples());
  }

  void mixToWetBuffer(juce::AudioBuffer<float>& buffer) noexcept {
    jassert(buffer.getNumSamples() <= dryBuffer.getNumSamples());
    jassert(buffer.getNumChannels() <= dryBuffer.getNumChannels());

    wetGain.applyGain(buffer, buffer.getNumSamples());
    for (const auto channel : std::views::iota(0, buffer.getNumChannels())) {
      buffer.addFrom(channel, 0, dryBuffer, channel, 0, buffer.getNumSamples());
    }
  }

  void reset() noexcept {
    setBypassForced(false);
    dryBuffer.clear();
  }

private:
  [[nodiscard]] bool isBypassed() const noexcept {
    return juce::exactlyEqual(dryGain.getTargetValue(), 1.0f);
  }

  double crossfadeLengthSeconds = 0.0;
  double sampleRateHz = 0.0;
  juce::LinearSmoothedValue<float> dryGain{0.f};
  juce::LinearSmoothedValue<float> wetGain{1.f};
  juce::AudioBuffer<float> dryBuffer;
};
}  // namespace tremolo
