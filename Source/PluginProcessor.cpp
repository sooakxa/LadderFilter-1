/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterAudioProcessor::LadderFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),treeState(*this, nullptr, Identifier("PARAMETERS"),
						   { std::make_unique<AudioParameterFloat>("cutoff", "Cutoff", 20.0f, 20000.0f, 20000.0f),
							 std::make_unique<AudioParameterFloat>("resonance", "Resonance", 0.0f, 1.10f, 0.15f),
							 std::make_unique<AudioParameterFloat>("drive", "Drive", 1.0f, 25.0f, 1.0f),
							 std::make_unique<AudioParameterChoice>("mode", "Filter Type", StringArray("LPF12", "LPF24", "HPF12", "HPF24"), 0) }) // Setup the tree state with initial values, including the parameters
#endif
{

	const StringArray params = { "cutoff", "resonance", "drive", "mode" };

	for (int i = 0; i <= 3; ++i)
	{
		// adds a listener to each parameter in the array.
		treeState.addParameterListener(params[i], this); 
	}

}

LadderFilterAudioProcessor::~LadderFilterAudioProcessor()
{
}

//==============================================================================
const String LadderFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LadderFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LadderFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LadderFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LadderFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LadderFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String LadderFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void LadderFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LadderFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Setup a processor spec to provide some context and info for the filter
	dsp::ProcessSpec spec;

	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();

	ladderFilter.reset();
	ladderFilter.prepare(spec);
	ladderFilter.setEnabled(true);
}

void LadderFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LadderFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void LadderFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// Process the audio in the buffer using the filter
	juce::dsp::AudioBlock<float> block(buffer);
	auto processingContext = dsp::ProcessContextReplacing<float>(block);
	ladderFilter.process(processingContext);
}

//==============================================================================
bool LadderFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LadderFilterAudioProcessor::createEditor()
{
    return new LadderFilterAudioProcessorEditor (*this, treeState);
}

//==============================================================================
void LadderFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LadderFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (treeState.state.getType()))
            treeState.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LadderFilterAudioProcessor();
}

// Function called when parameter is changed
void LadderFilterAudioProcessor::parameterChanged(const String& parameterID, float newValue) 
{
	if (parameterID == "cutoff")
		ladderFilter.setCutoffFrequencyHz(newValue);

	else if (parameterID == "resonance")
		ladderFilter.setResonance(newValue);

	else if (parameterID == "drive")
		ladderFilter.setDrive(newValue);

	else if (parameterID == "mode")
	{
		switch ((int)newValue)
		{
		case 0: ladderFilter.setMode(juce::dsp::LadderFilter<float>::Mode::LPF12);
			break;
		case 1: ladderFilter.setMode(juce::dsp::LadderFilter<float>::Mode::LPF24);
			break;
		case 2: ladderFilter.setMode(juce::dsp::LadderFilter<float>::Mode::HPF12);
			break;
		case 3: ladderFilter.setMode(juce::dsp::LadderFilter<float>::Mode::HPF24);
			break;
		}
	}
}