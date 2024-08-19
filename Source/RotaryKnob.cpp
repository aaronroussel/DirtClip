/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 17 Aug 2024 9:58:07pm
    Author:  Aaron Roussel

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
                        juce::AudioProcessorValueTreeState& apvts,
                        const juce::ParameterID& parameterID)
        : attachment(apvts, parameterID.getParamID(), slider)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setBounds(0, 0, 70, 86);

    slider.textFromValueFunction = [](double value)
    {
        return juce::String(value, 2);
    };

    addAndMakeVisible(slider);

    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{ 0, 0, 2, 0 });
    addAndMakeVisible(label);

    setSize(75, 110);

}

RotaryKnob::~RotaryKnob()
{

}

void RotaryKnob::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    slider.setTopLeftPosition(0, 24);

}
