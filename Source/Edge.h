/*
  ==============================================================================

    Edge.h
    Created: 2 Sep 2020 1:05:07pm
    Author:  gabey

  ==============================================================================
*/

#pragma once
#include "Point.h"

class Edge {
public:
	GB::Point start;
	GB::Point skewPoint;
	int resolution;
	double skew;
	std::vector<double> skewData;

	Edge(double x, double y);
	Edge(double, double, double);

	double readData(double t);

	void setSkewPoint();
	void setSkew(double newSkew);

	void moveStart(double x, double y);

	Edge* getNext();
	Edge* getPrev();
	void setNext(Edge *n);
	void setPrev(Edge *p);
	bool mouseDown(const MouseEvent &event, juce::Rectangle<int>);
	bool mouseDrag(const MouseEvent &event, juce::Rectangle<int>);
	void paintEdge(Graphics& g, juce::Rectangle<int>);
	void paint(Graphics& g, juce::Rectangle<int>);
	void backgroundPaint(Graphics& g, juce::Rectangle<int>);
	String toString();

private:
	Edge *next;
	Edge *prev;
};