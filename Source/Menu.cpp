/*
  ==============================================================================

    Menu.cpp
    Created: 4 Sep 2020 12:38:48pm
    Author:  gabey

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Menu.h"

//==============================================================================
Menu::Menu(EnvelopeWindow *e) : buttonList(e)
{
	view.setViewedComponent(&buttonList, false);
	add.onClick = [this] {addOsc();};
	remove.onClick = [this] {removeOsc();};
	addAndMakeVisible(add);
	addAndMakeVisible(remove);
	addAndMakeVisible(view);
}

Menu::~Menu()
{
}

void Menu::paint (Graphics& g)
{
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);
}

void Menu::addOsc()
{
	buttonList.addOsc();
}

void Menu::removeOsc()
{
	buttonList.removeOsc();
}

void Menu::resized()
{
	auto viewBounds = getLocalBounds();
	auto buttonView = viewBounds.removeFromBottom(50);
	auto addView = buttonView.removeFromLeft(buttonView.getWidth() / 2);
	auto removeView = buttonView;
	view.setBounds(viewBounds);
	add.setBounds(addView);
	remove.setBounds(removeView);
}
