/*
  ==============================================================================

    ButtonList.cpp
    Created: 4 Sep 2020 12:52:25pm
    Author:  gabey

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ButtonList.h"

//==============================================================================
ButtonList::ButtonList(EnvelopeWindow *e) {
	window = e;
	for (int i = 0; i < max_envelopes; ++i) {
		buttonBank.push_back(new GB::ToggleButton(window, this));
		addChildComponent(buttonBank[i]);
		buttonBank[i]->setIndex(i);
	}
	update();
}

ButtonList::~ButtonList()
{
	for (int i = 0; i < buttonBank.size(); ++i) {
		delete buttonBank[i];
	}
}

//sets all buttons off except index
void ButtonList::resetButtons(int index) {
	for (int i = 0; i < buttonBank.size(); ++i) {
		if (i != index) buttonBank[i]->turnOff();
	}
}

void ButtonList::paint (Graphics& g)
{
}

void ButtonList::update() {
	//updates button list to make sure it's reflecting internal state
	bool updateScreen = false;
	for (int i = window->envPointer->size(); i < max_envelopes; ++i) {
		if (buttonBank[i]->isVisible()) {
			buttonBank[i]->setVisible(false);
			updateScreen = true;
		}
	}
	for (int i = 0; i < window->envPointer->size(); ++i) {
		if (i != window->selected_envelope) {
			buttonBank[i]->turnOff();
		}
		if (!buttonBank[i]->isVisible()) {
			buttonBank[i]->setVisible(true);
			updateScreen = true;
		}
		if (buttonBank[i]->frequency != (*(window->envPointer))[i]->frequency) {
			buttonBank[i]->setFrequency((*(window->envPointer))[i]->frequency);
		}
	}
	if (updateScreen) {
		setBounds(0, 0, 150, window->envPointer->size() * 40);
	}
}

void ButtonList::resized()
{
	for (int i = 0; i < buttonBank.size(); ++i) {
		buttonBank[i]->setBounds(25, 10 + 40 * i, 150, 30);
	}
}

void ButtonList::addOsc() {
	window->addEnvelope();
	update();
}

void ButtonList::removeOsc() {
	int index = window->selected_envelope;
	if (index >= 0) {
		window->selected_envelope = -1;
		window->removeEnvelope(index);
	}
	update();
}

void ButtonList::updateToggleState(int i) {
	window->selected_envelope = i;
	update();
}
