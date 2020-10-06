/*
  ==============================================================================

	SynthVoice.h
	Created: 24 Aug 2020 3:32:05pm
	Author:  gabey

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "Envelope.h"
#include "SineOsc.h"

class EnvelopeSynthAudioProcessor;

class SynthVoice : public SynthesiserVoice
{
public:
	SynthVoice(std::vector<Envelope *>& e, EnvelopeSynthAudioProcessor *p);

	bool canPlaySound(SynthesiserSound *s);

	void startNote(int midiNoteNumber, float velocity,
		juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;

	void stopNote(float /*velocity*/, bool allowTailOff) override;

	void pitchWheelMoved(int) override;
	void controllerMoved(int, int) override;

	void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples);

private:
	EnvelopeSynthAudioProcessor *processor;
	std::vector<SineOsc> oscBank;
	bool notePressed;
	bool tailPlaying;
	double level = 0.7;
	std::vector<Envelope*>* envPointer;
};