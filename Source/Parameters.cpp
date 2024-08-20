/*
  ==============================================================================

    Parameters.cpp
    Created: 17 Aug 2024 1:04:13pm
    Author:  Aaron Roussel

  ==============================================================================
*/

#include "Parameters.h"
#include "juce_core/system/juce_PlatformDefs.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                            const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}


Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, distortionParamID, distortionParam);
    castParameter(apvts, distortionSkewParamID, distortionSkewParam);
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    distortionSkewSmoother.setTargetValue(distortionSkewParam->get() * 0.01f);
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    distortionSkewSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;

    gainSmoother.setCurrentAndTargetValue(
        juce::Decibels::decibelsToGain(gainParam->get())
    );

    mix = 1.0f;

    mixSmoother.setCurrentAndTargetValue(
        mixParam->get() * 0.01f
    );

    distortion = 2;

    distortionSkew = 1.5f;

    distortionSkewSmoother.setCurrentAndTargetValue(
        distortionSkewParam->get() * 0.01f
    );
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    mix = mixSmoother.getNextValue();
    distortionSkew = distortionSkewSmoother.getNextValue();
}
