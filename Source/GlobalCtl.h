/*
  ==============================================================================

    GlobalCtl.h
    Created: 15 Sep 2020 3:57:19pm
    Author:  gabey

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


class GainSlider : public Slider
{
public:
	GainSlider() {
		setRange(-40.0, 4.0);
		setSliderStyle(juce::Slider::RotaryVerticalDrag);
		setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	}
	double getVal() {
		return juce::Decibels::decibelsToGain(getValue(), -39.9);
	}

	void setVal(double gain) {
		setValue(juce::Decibels::gainToDecibels(gain));
	}
};

class FrequencyBox : public Slider {
public:
	FrequencyBox() {
		setRange(1.0, 10000.0);
		setTextBoxStyle(juce::Slider::TextBoxBelow, false,
			getLocalBounds().getWidth(), getLocalBounds().getHeight());
		setSliderStyle(Slider::SliderStyle::LinearBarVertical);
		setSliderSnapsToMousePosition(false);
		setColour(Slider::trackColourId, Colours::transparentBlack);
	}
};

//==============================================================================
/*
*/
class GlobalCtl    : public Component, Slider::Listener
{
public:
    GlobalCtl(EnvelopeSynthAudioProcessor* p);
    ~GlobalCtl();

	void sliderValueChanged(Slider *slider);

    void paint (Graphics&) override;
    void resized() override;

private:
	GainSlider gain;
	FrequencyBox frequency;
	EnvelopeSynthAudioProcessor* procPointer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalCtl)
};
