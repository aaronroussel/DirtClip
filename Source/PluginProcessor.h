/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>
#include "Parameters.h"

class Distortion
{
public:
    enum class DistortionType
    {
        SoftClipping,
        HardClipping,
        Rectifier,
    };

    Distortion()
        : currentType(DistortionType::SoftClipping), gain(2.0f)
    {
    }

    void setDistortionType(DistortionType newType)
    {
        currentType = newType;
    }

    void setGain(float newGain)
    {
        gain = newGain;
    }

    float processSample(float inputSample)
    {
        inputSample *= gain;

        switch(currentType)
        {
            case DistortionType::SoftClipping:
                return softClipping(inputSample);
            case DistortionType::Rectifier:
                return rectifier(inputSample);
            default:
                return inputSample;
        }
    }

    void updateDistortionType(int value)
    {
        switch (value)
        {
            case 0:
                currentType = DistortionType::SoftClipping;
            case 1:
                currentType = DistortionType::Rectifier;
            case 2:
                currentType = DistortionType::HardClipping;
            default:
                currentType = DistortionType::SoftClipping;
        }
    }

private:
    DistortionType currentType;
    float gain;
    float skew;

    float softClipping(float sample)
    {
        return std::tanh(sample * gain);
    }

    float rectifier(float sample)
    {
      return pow(std::tanh(sample * gain), 6);
    }

    float hardClipping(float sample)
    {
        float processedSample  = 0.0f;

        if (sample < 1.0f / skew && sample > -1.0f / skew)
        {
            processedSample = sample * skew;
        }
        else if (sample <= -1.0f / skew)
        {
            processedSample = -1.0f;
        }
        else if (sample >= 1.0f / skew)
        {
            processedSample = 1.0f;
        }

        return processedSample;
    }
};


//==============================================================================
/**
*/
class DirtClipAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DirtClipAudioProcessor();
    ~DirtClipAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    juce::AudioProcessorValueTreeState apvts {
        *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };


private:
    Distortion distortion;

    Parameters params;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DirtClipAudioProcessor)
};
