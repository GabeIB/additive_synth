/*
  ==============================================================================

    SaveLoadMenu.cpp
    Created: 14 Sep 2020 1:25:01pm
    Author:  gabey

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SaveLoadMenu.h"
#include <string>

//==============================================================================
SaveLoadMenu::SaveLoadMenu(EnvelopeWindow * window, ButtonList *buttonlist)
{
	buttonList = buttonlist;
	e = window;
	load.onClick = [this] {loadFile();};
	save.onClick = [this] {saveFile();};
	addAndMakeVisible(load);
	addAndMakeVisible(save);
}

SaveLoadMenu::~SaveLoadMenu()
{
}

void SaveLoadMenu::paint (Graphics& g)
{
}

void SaveLoadMenu::loadFile(){
	FileChooser chooser{ "Load Preset" };
	if (chooser.browseForFileToOpen()) {
		std::string s = chooser.getResult().getFullPathName().toStdString();
		e->loadParams(s);
		buttonList->update();
	}
	
}

void SaveLoadMenu::saveFile(){
	FileChooser chooser{ "Save Preset" };
	if (chooser.browseForFileToSave(true)) {
		std::string s = chooser.getResult().getFullPathName().toStdString();
		e->saveParams(s);
	}
}

void SaveLoadMenu::resized()
{
	auto area = getLocalBounds();
	auto loadBounds = area.removeFromLeft(area.getWidth() / 2);
	load.setBounds(loadBounds.reduced(4));
	save.setBounds(area.reduced(4));
}
