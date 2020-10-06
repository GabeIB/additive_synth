/*
  ==============================================================================

    SineOsc.cpp
    Created: 7 Sep 2020 3:04:33pm
    Author:  gabey

  ==============================================================================
*/

#include "SineOsc.h"
#include <cmath>
#include "../JuceLibraryCode/JuceHeader.h"

float *SineOsc::static_table = nullptr;

SineOsc::SineOsc() : WaveTable::WaveTable(tableSize)
{
	tableSize = static_tableSize;
	if (static_table == nullptr) {
		static_table = new float[tableSize];
		for (int i = 0; i < tableSize; ++i) {
			static_table[i] =
				std::sin(double(i) * 2.0 * juce::MathConstants<double>::pi / double(tableSize + 1));
		}
	}
	table = static_table;
}
