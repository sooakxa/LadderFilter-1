/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterAudioProcessorEditor::LadderFilterAudioProcessorEditor (LadderFilterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), treeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(230, 330);

    // Cutoff Frequency
    cutoffValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "cutoff", cutoffDial);
    cutoffDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    cutoffDial.setRange(20.0f, 20000.0f, 0.0f);
    cutoffDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&cutoffDial);

    // Resonance
    resonanceValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "resonance", resonanceDial);
    resonanceDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    resonanceDial.setRange(0.0f, 1.10f, 0.0f);
    resonanceDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&resonanceDial);

    // Drive
    driveValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "drive", driveDial);
    driveDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveDial.setRange(1.0f, 25.0f, 1.0f);
    driveDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&driveDial);

    // Mode Selection
    modeSel.addItem("LPF12", 1);
    modeSel.addItem("LPF24", 2);
    modeSel.addItem("HPF12", 3);
    modeSel.addItem("HPF24", 4);
    modeSel.addItem("BPF12", 5);
    modeSel.addItem("BPF24", 6);
    modeChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "mode", modeSel);
    addAndMakeVisible(&modeSel);

    enabled.setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
    enabled.setButtonText("Set Enabled?");
    enabledToggle = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(treeState, "enabled", enabled);
    addAndMakeVisible(&enabled);
}

LadderFilterAudioProcessorEditor::~LadderFilterAudioProcessorEditor()
{
}

//==============================================================================
void LadderFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::aqua);
    g.setColour(juce::Colours::black);

    // Title Text
    g.setFont(30);
    g.drawFittedText("Ladder Filter", 10, 20, 210, 10, juce::Justification::centred, 1, 0.0f);

    // Frequnecy, Resonance & Drive labels
    g.setFont(25);
    g.drawFittedText("F", 55, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("R", 165, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("D", 55, 175, 12, 12, juce::Justification::centred, 1, 0.0f);

}

void LadderFilterAudioProcessorEditor::resized()
{
    cutoffDial.setBounds(10, 40, 100, 100);
    resonanceDial.setBounds(120, 40, 100, 100);
    driveDial.setBounds(10, 130, 100, 100);
    modeSel.setBounds(127.5, 169.5, 75, 25);
    enabled.setBounds(10, 220, 100, 100);
}
