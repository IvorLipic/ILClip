/*
  ==============================================================================

    This project was inspired by https://gvst.uk/Downloads/GClip

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(ILClipAudioProcessor& p) : processor(p)
{
    displayBuffer.setSize(1, processor.getSampleRate()); // Match the processor's circular buffer size
    startTimerHz(60); // Refresh at 60Hz
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::updateWaveform() 
{
    // Update threshold and make a copy of the new buffer
    threshold = juce::jlimit(0.0f, 1.0f, processor.tree.getRawParameterValue("Threshold") -> load());
    const auto& processorBuffer = processor.getWaveformBuffer();
    const auto* processorData = processorBuffer.getReadPointer(0);

    int numSamples = processorBuffer.getNumSamples();
    displayBuffer.copyFrom(0, 0, processorData, numSamples);
}

void WaveformDisplay::timerCallback()
{
    updateWaveform();
    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black); // Clear background

    const int width = getWidth();
    const int height = getHeight();

    // Draw waveform
    if (displayBuffer.getNumSamples() > 0)
    {
        g.setColour(juce::Colours::cyan);

        auto* samples = displayBuffer.getReadPointer(0);

        juce::Path waveformPath;
        waveformPath.startNewSubPath(0.0f, height / 2.0f); // Start in the middle

        for (int x = 0; x < width; ++x)
        {
            const int sampleIndex = juce::jmap(x, 0, width, 0, displayBuffer.getNumSamples());
            const float sample = juce::jlimit(-1.0f, 1.0f, samples[sampleIndex]);
            const float y = juce::jmap(sample, -1.0f, 1.0f, (float)height, 0.0f);

            waveformPath.lineTo((float)x, y);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(1.0f));
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
