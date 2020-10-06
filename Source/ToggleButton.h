/*
  ==============================================================================

    ToggleButton.h
    Created: 4 Sep 2020 2:09:27pm
    Author:  gabey

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeWindow.h"

class ButtonList;

//==============================================================================
/*
*/
namespace GB {
	class ToggleButton : public juce::Component, public juce::Button::Listener,
		public juce::Label::Listener
	{
	public:
		ToggleButton(EnvelopeWindow *w, ButtonList *g);
		void turnOff();
		void resized();
		void setIndex(int i);
		void mouseDoubleClick(const MouseEvent &event);
		void buttonClicked(juce::Button* button);
		void labelTextChanged(Label *l);
		void setFrequency(double d);
		double frequency = 440.0;
		int index = -1;

	private:
		ButtonList *group;
		
		EnvelopeWindow *window;
		juce::Label input;
		juce::ToggleButton b;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToggleButton)
	};
}