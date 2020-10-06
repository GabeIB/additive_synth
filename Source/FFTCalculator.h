/*
  ==============================================================================

    FFTCalculator.h
    Created: 9 Sep 2020 5:42:25pm
    Author:  gabey

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FFTCalculator {
public:
	FFTCalculator(int order, juce::dsp::WindowingFunction<float>::WindowingMethod, int sampleRate);
	~FFTCalculator();
	void newData(const float *d);
	float toFreq(int);
	std::vector<double> topFrequencies(int);
	std::vector<std::vector<double>> topPerOctave(int);
	double getAmplitudeAt(double);
	double freqToIndex(double f);
	std::vector<std::vector<double>> getLocalMaxima();
	std::vector<double> getAmplitudes(std::vector<double>);
	void performFFT();
private:
	int order;
	int size;
	int sampleRate;
	juce::dsp::WindowingFunction<float>::WindowingMethod window;
	float* data;
};