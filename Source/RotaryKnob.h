/*
  ==============================================================================

    RotaryKnob.h
    Created: 17 Aug 2024 9:58:07pm
    Author:  Aaron Roussel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text,
                juce::AudioProcessorValueTreeState& apvts,
                const juce::ParameterID& parameterID);
    ~RotaryKnob() override;

    juce::Slider slider;
    juce::Label label;
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
