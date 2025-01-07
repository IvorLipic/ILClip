/*
  ==============================================================================

    This project was inspired by https://gvst.uk/Downloads/GClip

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

ILClipAudioProcessor::ILClipAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ILClipAudioProcessor::~ILClipAudioProcessor()
{
}

const juce::String ILClipAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ILClipAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ILClipAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ILClipAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ILClipAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ILClipAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ILClipAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ILClipAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ILClipAudioProcessor::getProgramName (int index)
{
    return {};
}

void ILClipAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ILClipAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ILClipAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ILClipAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// Clipping functions
float ILClipAudioProcessor::hardClip(float sample, float threshold)
{
    if (sample > threshold)
        return threshold;
    if (sample < -threshold)
        return -threshold;
    return sample;
}

float ILClipAudioProcessor::softClipTanh(float sample, float threshold)
{
    return std::tanh(sample / threshold) * threshold;
}

float ILClipAudioProcessor::sShapedSigmoid(float sample, float threshold)
{
    float x = sample / threshold;
    return (x / (1.0f + std::abs(x))) * threshold;
}

float ILClipAudioProcessor::polynomialSoftClip(float sample, float threshold)
{
    float x = sample / threshold;
    if (std::abs(x) <= 1.0f)
        return x - (1.0f / 3.0f) * std::pow(x, 3.0f) * threshold;
    return (x > 0.0f ? 1.0f : -1.0f) * threshold;
}


void ILClipAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get parameters
    float threshold = *tree.getRawParameterValue("Threshold");
    int algorithmChoice = static_cast<int>(*tree.getRawParameterValue("AlgorithmChoice"));

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // Process each sample
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            switch (algorithmChoice)
            {
            case 0: // Hard Clip
                channelData[sample] = hardClip(inputSample, threshold);
                break;
            case 1: // Soft Clip (Tanh)
                channelData[sample] = softClipTanh(inputSample, threshold);
                break;
            case 2: // S-Shaped Sigmoid Clip
                channelData[sample] = sShapedSigmoid(inputSample, threshold);
                break;
            case 3: // Polynomial Soft Clip
                channelData[sample] = polynomialSoftClip(inputSample, threshold);
                break;
            default:
                break;
            }
        }
    }
    // Update the waveform display
    if (auto* editor = dynamic_cast<ILClipAudioProcessorEditor*>(getActiveEditor()))
    {
        editor->getWaveformDisplay().updateWaveform(buffer, *tree.getRawParameterValue("Threshold"));
    }
}

//==============================================================================
bool ILClipAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ILClipAudioProcessor::createEditor()
{
    return new ILClipAudioProcessorEditor (*this);
}

//==============================================================================
void ILClipAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes // ValueTree in this case
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream mos(destData, true);
    tree.state.writeToStream(mos);
}

void ILClipAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    auto valueTree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (valueTree.isValid()) {
        tree.replaceState(valueTree);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout ILClipAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Threshold",
        "Threshold",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 1.0f),
        0.0f));
    /*
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "UseNoteDuration",
        "UseNoteDuration",
        false));
    */

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "AlgorithmChoice",
        "AlgorithmChoice",
        juce::StringArray{ "Hard Clip", "Soft Clip with Hyperbolic Tangent", "S-Shaped Sigmoid Clip", "Polynomial Soft Clip" },
        0));
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ILClipAudioProcessor();
}
