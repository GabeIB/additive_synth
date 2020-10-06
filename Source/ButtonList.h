/*
  ==============================================================================

    ButtonList.h
    Created: 4 Sep 2020 12:52:25pm
    Author:  gabey

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeWindow.h"
#include "ToggleButton.h"

//==============================================================================
/*
*/
class ButtonList    : public Component
{
public:
    ButtonList(EnvelopeWindow *e);
    ~ButtonList();

    void paint (Graphics&) override;
    void resized() override;

	void addOsc();
	void removeOsc();
	void resetButtons(int);
	void update();

private:
	const int max_envelopes = 64; //temporary
	EnvelopeWindow *window;
	std::vector<GB::ToggleButton*> buttonBank;
	void updateToggleState(int);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonList)
};
