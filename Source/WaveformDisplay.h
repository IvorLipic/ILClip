/*
  ==============================================================================

    WaveformDisplay.h
    Created: 7 Jan 2025 12:19:12am
    Author:  Ivor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component
{
public:
    WaveformDisplay(ILClipAudioProcessor& p);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void updateWaveform(const juce::AudioBuffer<float>& buffer, float newThreshold);

private:
    ILClipAudioProcessor& processor;
    juce::CriticalSection updateLock;

    juce::AudioBuffer<float> waveformBuffer;
    float threshold = 0.0f;
    std::atomic<bool> needsRepaint{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
