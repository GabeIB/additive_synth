/*
  ==============================================================================

    EnvelopeWindow.h
    Created: 2 Sep 2020 12:32:56pm
    Author:  gabey

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"
#include <vector>
#include <string>

//==============================================================================
/*
*/
class EnvelopeWindow    : public Component, public Timer
{
public:
	int selected_envelope;
    EnvelopeWindow(std::vector<Envelope*>&);
    ~EnvelopeWindow();

    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;

	void mouseDown(const MouseEvent &) override;
	void mouseDoubleClick(const MouseEvent &) override;
	void mouseDrag(const MouseEvent &) override;
	void mouseUp(const MouseEvent &) override;
	bool keyPressed(const KeyPress &) override;
	void addEnvelope();
	void addEnvelope(double freq);
	void removeEnvelope(int);
	void setFrequency(int, double);
	void clear();
	void addData(int envelope, double x, double y);
	void addData(int envelope, double x, double y, double skew);
	void saveParams(std::string url);
	void loadParams(std::string url);
	std::vector<double> parseEdge(std::string s);
	std::vector<Envelope*>* envPointer;

private:
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeWindow)
};
