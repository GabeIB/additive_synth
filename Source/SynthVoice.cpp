/*
  ==============================================================================

    SynthVoice.cpp
    Created: 15 Sep 2020 4:46:50pm
    Author:  gabey

  ==============================================================================
*/

#include "SynthVoice.h"
#include "PluginProcessor.h"

SynthVoice::SynthVoice(std::vector<Envelope *>& e, EnvelopeSynthAudioProcessor *p) {
	processor = p;
	envPointer = &e;
	notePressed = false;
	tailPlaying = false;
	for (int i = 0; i < 64; ++i) {
		oscBank.push_back(SineOsc());
	}
}

bool SynthVoice::canPlaySound(SynthesiserSound *s) {
	return (dynamic_cast<SynthSound*>(s) != nullptr);
}

void SynthVoice::startNote(int midiNoteNumber, float velocity,
	juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
	auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	for (int i = 0; i < envPointer->size();++i) {
		(*envPointer)[i]->time = 0.0;
		(*envPointer)[i]->setSampleRate(getSampleRate());
		oscBank[i].setFrameRate(getSampleRate());
		float frequency = (*envPointer)[i]->frequency * cyclesPerSecond / processor->baseFreq;
		if (frequency < 20000) {
			oscBank[i].setFrequency(frequency);
		}
		else {
			oscBank[i].setFrequency(0);
		}
	}
	tailPlaying = true;
	notePressed = true;
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff) 
{
	for (int i = 0; i < envPointer->size();++i) {
		oscBank[i].setAmplitude(0.0f);
	}
	clearCurrentNote();
	notePressed = false;
	tailPlaying = true;
}

void SynthVoice::pitchWheelMoved(int) {}
void SynthVoice::controllerMoved(int, int) {}

void SynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
	int startSample, int numSamples)
{
	if (tailPlaying) {
		while (--numSamples >= 0) // [6]
		{
			float currentSample = 0.0f;
			for (int i = 0; i < envPointer->size();++i) {
				if (notePressed) {
					float relativeLevel = (*envPointer)[i]->getNext();
					oscBank[i].setAmplitude(relativeLevel);
				}
				float waveForm = oscBank[i].getNextSample();
				currentSample += waveForm;
			}
			if (!notePressed && currentSample < .00001) tailPlaying = false;
			currentSample *= processor->globalGain;

			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, currentSample);

			++startSample;
		}
	}
}