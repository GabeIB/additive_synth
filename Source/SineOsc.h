/*
  ==============================================================================

    SineOsc.h
    Created: 7 Sep 2020 3:04:33pm
    Author:  gabey

  ==============================================================================
*/

#pragma once
#include "Wavetable.h"

class SineOsc : public WaveTable
{
public:
	//framerate defaults to 44100 and tablesize defaults to 128
	SineOsc();

private:
	static const int static_tableSize = 16384;
	static float* static_table;
};
