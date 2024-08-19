/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DirtClipAudioProcessorEditor::DirtClipAudioProcessorEditor (DirtClipAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    inputGroup.setText("Input");
    inputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    inputGroup.addAndMakeVisible(flexKnob);
    addAndMakeVisible(inputGroup);

    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);


    setSize (700, 430);
}

DirtClipAudioProcessorEditor::~DirtClipAudioProcessorEditor()
{
}

//==============================================================================
void DirtClipAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkgrey);
}

void DirtClipAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    int y = 10;
    int height = bounds.getHeight() - 20;

    inputGroup.setBounds(10, y, 110, height);
    outputGroup.setBounds(bounds.getWidth() - 120, y, 110, height);


    flexKnob.setTopLeftPosition(20, 20);
    mixKnob.setTopLeftPosition(20, 20);

}
