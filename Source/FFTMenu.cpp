/*
  ==============================================================================

    FFTMenu.cpp
    Created: 9 Sep 2020 11:02:16am
    Author:  gabey

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FFTMenu.h"
#include "FFTCalculator.h"

//==============================================================================
FFTMenu::FFTMenu(EnvelopeWindow *w, ButtonList *b)
{
	buttonList = b;
	envBank = w;
	formatManager.registerBasicFormats();
	load.onClick = [this] {loadFile();};
	process.onClick = [this] {processFile();};
	addAndMakeVisible(load);
	addAndMakeVisible(process);
}

void FFTMenu::loadFile(){
	FileChooser chooser{ "please choose an audio file" };
	if (chooser.browseForFileToOpen()) {
		auto file = chooser.getResult();
		std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
		if (reader.get() != nullptr) {
			int duration = reader->sampleRate;
			if (reader->lengthInSamples < duration) duration = reader->lengthInSamples;
			file_sampleRate = reader->sampleRate;
			fileBuffer.setSize((int)reader->numChannels, duration);
			reader->read(&fileBuffer, 0, duration, 0, true, true); // loads 1 second
		}
	}
}

std::vector<double> FFTMenu::getInterestingFrequencies() {
	auto frequencyWindow = juce::dsp::WindowingFunction<float>::hamming;
	int order = (int)log2(fileBuffer.getNumSamples());
	FFTCalculator frequency(order, frequencyWindow, file_sampleRate);
	frequency.newData(fileBuffer.getReadPointer(0));
	frequency.performFFT();
	//return frequency.topFrequencies(1);
	std::vector<std::vector<double>> r = frequency.topPerOctave(3);
	std::vector<double> frequencies;
	for (int i = 0; i < r.size(); ++i) {
		frequencies.push_back(r[i][0]);
	}
	return frequencies;
}

std::vector<std::vector<double>> FFTMenu::getAmplitude(std::vector<double> frequencies, int order){
	auto amplitudeWindow = juce::dsp::WindowingFunction<float>::rectangular;
	std::vector<std::vector<double>> r(frequencies.size());
	FFTCalculator amplitude(order, amplitudeWindow, file_sampleRate);
	int readStart = 0;
	while (readStart + (1 << order) <= fileBuffer.getNumSamples()) {
		amplitude.newData(fileBuffer.getReadPointer(0, readStart));
		amplitude.performFFT();
		for (int i = 0; i < frequencies.size(); ++i) {
			r[i].push_back(amplitude.getAmplitudeAt(frequencies[i]));
		}
		/*
		std::vector<double> d = amplitude.getAmplitudes(frequencies);
		for (int i = 0; i < frequencies.size(); ++i) {
			r[i].push_back(d[i]);
		}*/
		readStart += (1 << order);
	}
	return r;
}

void FFTMenu::processFile(){
	//clear envelopes
	//set up new envelopes
	if (fileBuffer.getNumSamples() > 0) {
		envBank->clear();

		std::vector<double> interesting_frequencies = getInterestingFrequencies();
		for (int i = 0; i < interesting_frequencies.size(); ++i){
			envBank->addEnvelope(interesting_frequencies[i]);
			//envBank->addData(i, 0.5, 0.5);
		}
		double max_timestep = 0.05;
		int order = log2(max_timestep * file_sampleRate);
		double timeStep = double(1 << order) / file_sampleRate;
		std::vector<std::vector<double>> ctlData = getAmplitude(interesting_frequencies, order);
		for (int i = 0; i < ctlData.size(); ++i) {
			int j = 0;
			for (; j < ctlData[i].size(); ++j) {
				envBank->addData(i, j*timeStep, ctlData[i][j]);
			}
			if (!(j*timeStep > 1)) {
				envBank->addData(i, j*timeStep, 0.0);
			}
		}

		//envBank->addEnvelope(320.2);
		//envBank->addData(0, 0.5, 0.5);
		buttonList->update();
	}
}

FFTMenu::~FFTMenu()
{
}

void FFTMenu::paint (Graphics& g)
{
}

void FFTMenu::resized()
{
	auto area = getLocalBounds();
	auto loadBounds = area.removeFromLeft(area.getWidth() / 2);
	load.setBounds(loadBounds);
	process.setBounds(area);
}
