/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 7 Jan 2025 12:19:12am
    Author:  Ivor

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(ILClipAudioProcessor& p) : processor(p)
{
    waveformBuffer.setSize(1, 0);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::updateWaveform(const juce::AudioBuffer<float>& buffer, float newThreshold) {
    juce::ScopedLock lock(updateLock);

    // Update threshold and make a copy of the new buffer
    threshold = juce::jlimit(0.0f, 1.0f, newThreshold);
    waveformBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
    waveformBuffer.makeCopyOf(buffer);

    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black); // Clear background

    juce::ScopedLock lock(updateLock);

    const int width = getWidth();
    const int height = getHeight();

    // Draw waveform
    if (waveformBuffer.getNumSamples() > 0)
    {
        g.setColour(juce::Colours::cyan);

        for (int channel = 0; channel < waveformBuffer.getNumChannels(); ++channel)
        {
            auto* samples = waveformBuffer.getReadPointer(channel);
            juce::Path waveformPath;

            waveformPath.startNewSubPath(0.0f, height / 2.0f); // Start in the middle

            for (int x = 0; x < width; ++x)
            {
                const int sampleIndex = juce::jmap(x, 0, width, 0, waveformBuffer.getNumSamples());
                const float sample = juce::jlimit(-1.0f, 1.0f, samples[sampleIndex]);
                const float y = juce::jmap(sample, -1.0f, 1.0f, (float)height, 0.0f);

                waveformPath.lineTo((float)x, y);
            }

            g.strokePath(waveformPath, juce::PathStrokeType(1.0f));
        }
    }

    // Draw threshold lines
    const float topThresholdY = juce::jmap(-threshold, -1.0f, 1.0f, (float)height, 0.0f);
    const float bottomThresholdY = juce::jmap(threshold, -1.0f, 1.0f, (float)height, 0.0f);

    g.setColour(juce::Colours::red);
    g.drawLine(0.0f, topThresholdY, (float)width, topThresholdY, 1.0f);
    g.drawLine(0.0f, bottomThresholdY, (float)width, bottomThresholdY, 1.0f);
}


void WaveformDisplay::resized()
{
    // No child components to resize
}
