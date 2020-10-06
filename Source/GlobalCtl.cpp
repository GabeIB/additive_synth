/*
  ==============================================================================

    GlobalCtl.cpp
    Created: 15 Sep 2020 3:57:19pm
    Author:  gabey

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalCtl.h"

//==============================================================================
GlobalCtl::GlobalCtl(EnvelopeSynthAudioProcessor* p)
{
	procPointer = p;
	gain.setVal(p->globalGain);
	frequency.setValue(p->baseFreq);
	gain.addListener(this);
	frequency.addListener(this);
	addAndMakeVisible(gain);
	addAndMakeVisible(frequency);
}

GlobalCtl::~GlobalCtl()
{
}

void GlobalCtl::paint (Graphics& g)
{
}

void GlobalCtl::resized()
{
	auto area = getLocalBounds();
	gain.setBounds(area.removeFromRight(50));
	frequency.setBounds(area.removeFromRight(80));
}

void GlobalCtl::sliderValueChanged(Slider *slider) {
	if (slider == &gain) {
		procPointer->globalGain = gain.getVal();
	}
	else if (slider == &frequency) {
		procPointer->baseFreq = frequency.getValue();
	}
}