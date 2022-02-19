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
class LadderFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LadderFilterAudioProcessorEditor (LadderFilterAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~LadderFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider cutoffDial;
    juce::Slider resonanceDial;
    juce::Slider driveDial;
    juce::ComboBox modeSel;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> resonanceValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeChoice;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LadderFilterAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterAudioProcessorEditor)
};
