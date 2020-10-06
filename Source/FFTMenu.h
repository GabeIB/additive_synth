/*
  ==============================================================================

    FFTMenu.h
    Created: 9 Sep 2020 11:02:16am
    Author:  gabey

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeWindow.h"
#include "ButtonList.h"
#include <vector>

//==============================================================================
/*
*/
class FFTMenu    : public Component
{
public:
    FFTMenu(EnvelopeWindow *w, ButtonList *b);
    ~FFTMenu();

    void paint (Graphics&) override;
    void resized() override;
	void loadFile();
	void processFile();

	std::vector<double> getInterestingFrequencies();
	std::vector<std::vector<double>> getAmplitude(std::vector<double>, int);

private:
	AudioFormatManager formatManager;
	AudioBuffer<float> fileBuffer;
	double file_sampleRate;
	ButtonList *buttonList;
	EnvelopeWindow *envBank;
	juce::TextButton load{ "load" };
	juce::TextButton process{ "proc" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTMenu)
};
