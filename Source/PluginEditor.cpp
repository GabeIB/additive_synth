/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EnvelopeSynthAudioProcessorEditor::EnvelopeSynthAudioProcessorEditor (EnvelopeSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), window(p.envelopes), menu(&window),
	fft(&window, &(menu.buttonList)), global(&p)
{
	setSize(900, 500);
	addAndMakeVisible(menu);
	addMouseListener(&window, false);
	addAndMakeVisible(window);
	addAndMakeVisible(fft);
	addAndMakeVisible(global);
}

void EnvelopeSynthAudioProcessorEditor::updateToggleState(int i)
{
	window.selected_envelope = i;
}

EnvelopeSynthAudioProcessorEditor::~EnvelopeSynthAudioProcessorEditor()
{
}

//==============================================================================
void EnvelopeSynthAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void EnvelopeSynthAudioProcessorEditor::resized()
{
	auto windowArea = getLocalBounds();
	auto menuArea = windowArea.removeFromLeft(200);
	auto topBar = windowArea.removeFromTop(50);
	auto fftArea = topBar.removeFromLeft(150);
	auto globalArea = topBar.removeFromRight(200);
	window.setBounds(windowArea);
	menu.setBounds(menuArea);
	fft.setBounds(fftArea);
	global.setBounds(globalArea);
}
