/*
  ==============================================================================

    This project was inspired by https://gvst.uk/Downloads/GClip

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveformDisplay.h"

class ILClipAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ILClipAudioProcessorEditor (ILClipAudioProcessor&);
    ~ILClipAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ILClipAudioProcessor& audioProcessor;
     
    // Threshold slider
    juce::Slider thresholdSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment thresholdSlider_attachment;
    juce::Label thresholdSlider_label;

    // Alghorithm choice drop down
    juce::ComboBox algorithmChoice_comboBox;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment algorithmChoice_attachment;
    juce::Label algorithmChoice_label;

    // WaveformDisplay
    WaveformDisplay waveformDisplay;

public:
    WaveformDisplay& getWaveformDisplay();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ILClipAudioProcessorEditor)
};
