/*
==============================================================================

ToggleButton.cpp
Created: 8 Sep 2020 2:30:45pm
Author:  gabey

==============================================================================
*/

#include "ToggleButton.h"
#include "ButtonList.h"

//==============================================================================
/*
*/
GB::ToggleButton::ToggleButton(EnvelopeWindow *w, ButtonList *g) {
	group = g;
	addAndMakeVisible(b);
	b.setClickingTogglesState(true);
	b.addListener(this);
	input.addListener(this);
	addAndMakeVisible(input);
	input.setEditable(false, true);
	input.setText("440.0", juce::dontSendNotification);
	window = w;
}

void GB::ToggleButton::turnOff() {
	b.setToggleState(false, false);
}

void GB::ToggleButton::resized() {
	auto bounds = getLocalBounds();
	auto buttonArea = bounds.removeFromLeft(bounds.getHeight());
	auto textArea = bounds;
	b.setBounds(0, 0, buttonArea.getWidth(), buttonArea.getHeight());
	input.setBounds(buttonArea.getWidth(), 0, 80, textArea.getHeight());
}

void GB::ToggleButton::setIndex(int i) {
	index = i;
}

void GB::ToggleButton::mouseDoubleClick(const MouseEvent &event) {
	input.setVisible(true);
}

void GB::ToggleButton::buttonClicked(juce::Button* button) {
	if (window->selected_envelope != index) {
		window->selected_envelope = index;
		group->update();
	}
	else {
		window->selected_envelope = -1;
	}
}

void GB::ToggleButton::labelTextChanged(Label *l) {
	auto s = input.getText();
	auto cp = s.getCharPointer();
	double v = juce::CharacterFunctions::readDoubleValue(cp);
	if (cp != s.getCharPointer()) {
		//conversion was valid
		if (v > 0 && v < 20000) {
			frequency = v;
			window->setFrequency(index, frequency);
		}
	}
	input.setText(String(frequency), juce::dontSendNotification);
}

void GB::ToggleButton::setFrequency(double d) {
	frequency = d;
	input.setText(String(frequency), juce::dontSendNotification);
}
