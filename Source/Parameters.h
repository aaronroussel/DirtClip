/*
  ==============================================================================

    Parameters.h
    Created: 17 Aug 2024 1:04:13pm
    Author:  Aaron Roussel

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID { "gain", 1 };
const juce::ParameterID mixParamID { "mix", 1 };
const juce::ParameterID distortionParamID { "distortion", 1 };
const juce::ParameterID distortionSkewParamID { "distortionSkew", 1 };

class Parameters
{
    public:
        Parameters(juce::AudioProcessorValueTreeState& apvts);
        static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

        void update() noexcept;
        void prepareToPlay (double sampleRate) noexcept;
        void reset() noexcept;
        void smoothen() noexcept;

        float gain = 0.0f;
        float mix = 1.0f;
        int distortion = 0;
        float distortionSkew = 1.5f;
    private:
        juce::AudioParameterFloat* gainParam;
        juce::LinearSmoothedValue<float> gainSmoother;
        juce::AudioParameterFloat* mixParam;
        juce::LinearSmoothedValue<float> mixSmoother;
        juce::AudioParameterInt* distortionParam;
        juce::AudioParameterFloat* distortionSkewParam;
        juce::LinearSmoothedValue<float> distortionSkewSmoother;
};
