/*
  ==============================================================================

    This project was inspired by https://gvst.uk/Downloads/GClip

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, private juce::Timer
{
public:
    WaveformDisplay(ILClipAudioProcessor& p);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    

private:
    ILClipAudioProcessor& processor;
    float threshold = 0.0f;
    juce::AudioBuffer<float> displayBuffer;

    void updateWaveform();
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
