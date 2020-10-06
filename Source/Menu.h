/*
  ==============================================================================

    Menu.h
    Created: 4 Sep 2020 12:38:48pm
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

class Menu    : public Component
{
public:
    Menu(EnvelopeWindow*);
    ~Menu();

    void paint (Graphics&) override;
    void resized() override;
	void addOsc();
	void removeOsc();
	ButtonList buttonList;

private:
	Viewport view;
	TextButton add{ "add" };
	TextButton remove{ "remove" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Menu)
};
