/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LimitPluginAudioProcessor::LimitPluginAudioProcessor()
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

LimitPluginAudioProcessor::~LimitPluginAudioProcessor()
{
}

//==============================================================================
const juce::String LimitPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LimitPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LimitPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LimitPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LimitPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LimitPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LimitPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LimitPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LimitPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void LimitPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LimitPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	allBuffers = juce::Array<CircularBuffer>();
	for (int channel = 0; channel < getMainBusNumOutputChannels(); channel++)
	{
		allBuffers.add(CircularBuffer(10, 1));
	}

	limiterThresh = 0.8f;
	gain = 1.0f;
	xPeak = 0.0f;
}

void LimitPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LimitPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LimitPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	float coeff;

	for (int i = 0; i < buffer.getNumSamples(); i++)
	{
		for (int channel = 0; channel < getMainBusNumOutputChannels(); channel++)
		{
			auto* data = buffer.getWritePointer(channel);

			CircularBuffer* delayBuffer = &allBuffers.getReference(channel);

			float sample = data[i];

			float amplitude = abs(sample);

			if (amplitude > xPeak)
			{
				coeff = attackTime;
			}
			else
			{
				coeff = releaseTime;
			}

			xPeak = (1 - coeff)*xPeak + coeff * amplitude;

			float filter = fmin(1.0f, limiterThresh / xPeak);

			if (gain > filter)
			{
				coeff = attackTime;
			}
			else
			{
				coeff = releaseTime;
			}

			gain = (1 - coeff) * gain + coeff * filter;

			float limitedSample = gain * delayBuffer->getData();

			delayBuffer->setData(sample);
			delayBuffer->nextSample();

			data[i] = limitedSample;
		}

	}
}

//==============================================================================
bool LimitPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LimitPluginAudioProcessor::createEditor()
{
    return new LimitPluginAudioProcessorEditor (*this);
}

//==============================================================================
void LimitPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LimitPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LimitPluginAudioProcessor();
}
