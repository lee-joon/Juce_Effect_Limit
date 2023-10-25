/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class LimitPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LimitPluginAudioProcessorEditor (LimitPluginAudioProcessor&);
    ~LimitPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LimitPluginAudioProcessor& audioProcessor;
	juce::Slider threshold, at, rt;
	juce::Label tsLabel, atLabel, rtLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LimitPluginAudioProcessorEditor)
};
