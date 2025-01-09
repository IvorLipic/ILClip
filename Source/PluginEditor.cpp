/*
  ==============================================================================

    This project was inspired by https://gvst.uk/Downloads/GClip

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

ILClipAudioProcessorEditor::ILClipAudioProcessorEditor (ILClipAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), waveformDisplay (p),
    thresholdSlider_attachment(audioProcessor.tree, "Threshold", thresholdSlider),
    algorithmChoice_attachment(audioProcessor.tree, "AlgorithmChoice", algorithmChoice_comboBox)
{
    setSize(600, 400);

    // Threshold slider
    addAndMakeVisible(thresholdSlider);
    thresholdSlider_label.setText("Threshold", juce::dontSendNotification);
    thresholdSlider_label.setJustificationType(juce::Justification::centred);
    thresholdSlider_label.attachToComponent(&thresholdSlider, false);
    
    // Algorithm choice dropdown
    algorithmChoice_comboBox.addItemList({ "Hard Clip", "Soft Clip with Hyperbolic Tangent", 
                                           "S-Shaped Sigmoid Clip", "Polynomial Soft Clip" }, 1);
    addAndMakeVisible(algorithmChoice_comboBox);
    algorithmChoice_label.setText("Clipping algorithm", juce::dontSendNotification);
    algorithmChoice_label.setJustificationType(juce::Justification::centred);
    algorithmChoice_label.attachToComponent(&algorithmChoice_comboBox, false);

    // Waveform display window
    addAndMakeVisible(waveformDisplay);
}

ILClipAudioProcessorEditor::~ILClipAudioProcessorEditor()
{
}

void ILClipAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

// Bounds
void ILClipAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto alg_choice_area = bounds.removeFromLeft(0.25 * bounds.getWidth());
    auto params_area = bounds.removeFromLeft((1.0 / 3.0) * bounds.getWidth());
    auto waveform_area = bounds.removeFromTop(0.5 * bounds.getHeight());
    auto func_area = bounds;

    thresholdSlider.setBounds(params_area.removeFromTop(0.5 * params_area.getHeight())
                                         .removeFromBottom(0.3 * params_area.getHeight()));
    algorithmChoice_comboBox.setBounds(alg_choice_area.removeFromTop(0.5 * alg_choice_area.getHeight())
                                                      .removeFromBottom(0.3 * alg_choice_area.getHeight())
                                                      .reduced(5));
    waveformDisplay.setBounds(bounds.reduced(5));
}
