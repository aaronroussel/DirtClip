/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <memory>

//==============================================================================
DirtClipAudioProcessor::DirtClipAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       params(apvts)
#endif
{
}

DirtClipAudioProcessor::~DirtClipAudioProcessor()
{
}

//==============================================================================
const juce::String DirtClipAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DirtClipAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DirtClipAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DirtClipAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DirtClipAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DirtClipAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DirtClipAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DirtClipAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DirtClipAudioProcessor::getProgramName (int index)
{
    return {};
}

void DirtClipAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DirtClipAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
}

void DirtClipAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DirtClipAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void DirtClipAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.update();
    distortion.updateDistortionType(params.distortion);

    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        params.smoothen();

        float dryL = channelDataL[sample];
        float dryR = channelDataR[sample];

        float wetL = distortion.processSample(channelDataL[sample] * params.gain);
        float werR = distortion.processSample(channelDataR[sample] * params.gain);

        float mixL = dryL + wetL * params.mix;
        float mixR = dryL + wetL * params.mix;

        channelDataL[sample] = mixL;
        channelDataR[sample] = mixR;
    }
}

//==============================================================================
bool DirtClipAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DirtClipAudioProcessor::createEditor()
{
    return new DirtClipAudioProcessorEditor (*this);
}

//==============================================================================
void DirtClipAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void DirtClipAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DirtClipAudioProcessor();
}

//==============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout
    Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output Gain",
        juce::NormalisableRange<float> { -12.0f, 12.0f },
        0.0f));


    layout.add(std::make_unique<juce::AudioParameterFloat>(
        mixParamID,
        "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
    ));

    return layout;
}
