/*
  ==============================================================================

    Envelope.h
    Created: 2 Sep 2020 12:39:13pm
    Author:  gabey

  ==============================================================================
*/

#pragma once
#include "Edge.h"

class Envelope {
public:
	double time;
	double current_sample;
	double frequency;

	Envelope();
	Envelope(double freq);
	void addData(double, double);
	void addData(double, double, double);
	~Envelope();
	void paint(Graphics& g, juce::Rectangle<int> bounds);
	void backgroundPaint(Graphics& g, juce::Rectangle<int> bounds);
	double getNext();
	void setSampleRate(double);
	void mouseDown(const MouseEvent &, juce::Rectangle<int>);
	void mouseDoubleClick(const MouseEvent &, juce::Rectangle<int>);
	void mouseDrag(const MouseEvent &, juce::Rectangle<int>);
	void mouseUp(const MouseEvent &, juce::Rectangle<int>);
	bool keyPressed(const KeyPress &, juce::Rectangle<int>);
	String toString();

private:
	Edge *head;
	Edge *tail;
	Edge *current;
	double length;
	double sampleRate;
};