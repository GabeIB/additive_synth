/*
  ==============================================================================

    FFTCalculator.cpp
    Created: 9 Sep 2020 5:42:25pm
    Author:  gabey

  ==============================================================================
*/

#include "FFTCalculator.h"
#include <algorithm>
FFTCalculator::FFTCalculator(int o, juce::dsp::WindowingFunction<float>::WindowingMethod w, int sampleRate)
	: sampleRate(sampleRate), order(o), size(1 << order), window(w), data(new float[size * 2]) {}

FFTCalculator::~FFTCalculator() {
	delete[] data;
}

void FFTCalculator::newData(const float *d) {
	std::fill(data, data + (size * 2), 0.0f);
	std::copy(d, d + size, data);
}

void FFTCalculator::performFFT() {
	juce::dsp::WindowingFunction<float> window(size, window);
	juce::dsp::FFT forwardFFT(order);
	window.multiplyWithWindowingTable(data, size); //kind of hacky, maybe fix this later
	forwardFFT.performFrequencyOnlyForwardTransform(data);
	auto mindB = -100.0f;
	auto maxdB = 0.0f;
	for (int i = 0; i < size / 2; ++i)
	{
		float level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(data[i])
			- juce::Decibels::gainToDecibels((float)size)), mindB, maxdB, 0.0f, 1.0f);

		data[i] = level;
	}
}

double FFTCalculator::freqToIndex(double f){
	double i = (f / double(sampleRate))*double(size);
	return i;
}

double FFTCalculator::getAmplitudeAt(double freq) {
	double index = freqToIndex(freq);
	int i = int(index);
	double r = index - i;
	if (i < size / 2) {
		return data[i] * (1 - r) + data[i + 1] * r;
	}
	else {
		return data[i];
	}
}

float FFTCalculator::toFreq(int i) {
	return double(sampleRate) * double(i) / double(size);
}

std::vector<std::vector<double>> FFTCalculator::getLocalMaxima() {
	std::vector<std::vector<double>> r;
	for (int i = 1; i < size / 2; ++i) {
		if (data[i] > data[i - 1] && data[i] > data[i + 1]) {
			std::vector<double> p = { toFreq(i), data[i] };
			r.push_back(p);
		}
	}
	return r;
}

std::vector<double> FFTCalculator::topFrequencies(int returnSize) {
	std::vector<std::vector<double>> maxima = getLocalMaxima();
	std::vector<double> frequencies;
	std::sort(maxima.begin(), maxima.end(), [](std::vector<double>a, std::vector<double>b) {return a[1] > b[1];});
	if (maxima.size() < returnSize) returnSize = maxima.size();
	for (int i = 0; i < returnSize; ++i) {
		frequencies.push_back(maxima[i][0]);
	}
	std::sort(frequencies.begin(), frequencies.end());
	return frequencies;
}

bool inSameOctave(double i, double j) {
	return int(log2(i / 20.0)) == int(log2(j / 20.0));
}

std::vector<double> FFTCalculator::getAmplitudes(std::vector<double> frequencies) {
	std::vector<std::vector<double>> localMax = topPerOctave(1);
	std::vector<double> r;
	int j = 0;
	for (int i = 0; i < frequencies.size(); ++i) {
		if (j < localMax.size() && inSameOctave(frequencies[i], localMax[j][0])) {
			r.push_back(localMax[j][1]);
			++j;
		}
		else {
			r.push_back(0.0);
		}
	}
	return r;
}

std::vector<std::vector<double>> FFTCalculator::topPerOctave(int perOctave) {
	std::vector<std::vector<double>> maxima = getLocalMaxima();
	std::vector<std::vector<double>> r;
	double start = 20.0; //hz
	double end = start * 2;
	std::vector<std::vector<double>> top(perOctave);
	for (int i = 0; i < top.size(); ++i) {
		top[i].push_back(0.0);
		top[i].push_back(0.0);
	}
	auto it = maxima.begin();
	while (end <= 20000) {
		std::sort(maxima.begin(), maxima.end(), [](std::vector<double>a, std::vector<double>b) {return a[0] < b[0];});
		while (it < maxima.end()) {
			if ((*it)[0] >= start) {
				if ((*it)[0] > end) break;
				if ((*it)[1] > top[perOctave - 1][1]) {
					top[perOctave - 1][0] = (*it)[0];
					top[perOctave - 1][1] = (*it)[1];
					std::sort(top.begin(), top.end(),
						[](std::vector<double>a, std::vector<double>b) {return a[1] > b[1];});
				}
			}
			++it;
		}
		for (int i = 0; i < top.size(); ++i) {
			if (top[i][0] > 10.0) {
				r.push_back(top[i]);
			}
			top[i][0] = 0.0;
			top[i][1] = 0.0;
		}
		start = end;
		end = start * 2;
	}
	return r;
}