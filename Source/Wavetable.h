/*
  ==============================================================================

    Wavetable.h
    Created: 7 Sep 2020 3:04:22pm
    Author:  gabey

  ==============================================================================
*/

#pragma once
#include <cmath>
class WaveTable {
public:
	//framerate defaults to 44100 and tablesize defaults to 128
	~WaveTable();
	float getNextSample();
	void setFrequency(float);
	void setFrameRate(float);
	void setAmplitude(float);

protected:
	WaveTable(int tableSize);
	float targetAmp;
	float currentAmp;
	float frequency;
	float phase;
	float offset;
	int frameRate;
	int tableSize;
	float* table;
};