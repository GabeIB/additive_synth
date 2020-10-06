/*
  ==============================================================================

    Wavetable.cpp
    Created: 7 Sep 2020 3:04:22pm
    Author:  gabey

  ==============================================================================
*/

#include "Wavetable.h"
WaveTable::WaveTable(int tableSize)
	: frequency{ 1 }, phase{ 0 }, frameRate{ 0 }, tableSize{ tableSize }
{
	this->offset = (this->frequency*tableSize) / (frameRate);
}

WaveTable::~WaveTable() {
	//delete table;
}

void WaveTable::setFrameRate(float f) {
	frameRate = f;
}

void WaveTable::setFrequency(float f) {
	frequency = f;
	offset = (f*tableSize) / (frameRate);
}

void WaveTable::setAmplitude(float a) {
	targetAmp = a;
}

float WaveTable::getNextSample() {
	if (targetAmp != currentAmp) {
		float acceptableChange = 1000.0 / frameRate;
		if (acceptableChange > std::abs(targetAmp - currentAmp)) {
			currentAmp = targetAmp;
		}
		else {
			if (currentAmp > targetAmp) currentAmp -= acceptableChange;
			else currentAmp += acceptableChange;
		}
	}
	if (frequency > 0) {
		phase += offset;
		int i = int(phase);
		int i_1 = i + 1;
		if (i >= tableSize) {
			i -= tableSize;
			phase -= tableSize;
		}
		if (i_1 >= tableSize) i_1 -= tableSize;
		float remainder = phase - i;
		return (table[i] * (1 - remainder) + table[i_1] * (remainder))*currentAmp;
	}
	else {
		return 0.0f;
	}
}
