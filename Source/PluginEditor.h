/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "EnvelopeWindow.h"
#include "Menu.h"
#include "SaveLoadMenu.h"
#include "GlobalCtl.h"

//==============================================================================
/**
*/
class EnvelopeSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    EnvelopeSynthAudioProcessorEditor (EnvelopeSynthAudioProcessor&);
    ~EnvelopeSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void updateToggleState(int);

private:

    EnvelopeSynthAudioProcessor& processor;
	EnvelopeWindow window;
	Menu menu;
	SaveLoadMenu fft;
	GlobalCtl global;
	//FFTMenu fft;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeSynthAudioProcessorEditor)
};
