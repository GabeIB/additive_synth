/*
  ==============================================================================

    SaveLoadMenu.h
    Created: 14 Sep 2020 1:25:01pm
    Author:  gabey

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeWindow.h"
#include "ButtonList.h"

//==============================================================================
/*
*/
class SaveLoadMenu    : public Component
{
public:
    SaveLoadMenu(EnvelopeWindow *e, ButtonList *b);
    ~SaveLoadMenu();
	void loadFile();
	void saveFile();

    void paint (Graphics&) override;
    void resized() override;

private:
	ButtonList *buttonList;
	EnvelopeWindow *e;
	juce::TextButton load{ "load" };
	juce::TextButton save{ "save" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaveLoadMenu)
};
