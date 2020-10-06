/*
  ==============================================================================

    Point.cpp
    Created: 2 Sep 2020 12:39:55pm
    Author:  gabey

  ==============================================================================
*/

#include "Point.h"

GB::Point::Point(double x, double y) : x(x), y(y), radius(10), selected(false)
{}

GB::Point::Point(double x, double y, int radius) : x(x), y(y), radius(radius), selected(false) {}

bool GB::Point::withinPoint(double clickX, double clickY, juce::Rectangle<int> bounds) {
	double deltaX = toExternalX(x,bounds) - toExternalX(clickX,bounds);
	double deltaY = toExternalY(y,bounds) - toExternalY(clickY,bounds);
	if (square(deltaX) + square(deltaY) < square(radius)) return true;
	return false;
}

//returns true if point selected
bool GB::Point::mouseDown(const MouseEvent &event, juce::Rectangle<int> bounds) {
	if (withinPoint(toInternalX(event.getMouseDownX(), bounds), toInternalY(event.getMouseDownY(), bounds), bounds)) {
		selected = true;
		startX = x;
		startY = y;
		return true;
	}
	else {
		selected = false;
		return false;
	}
}

void GB::Point::movePoint(double x, double y) {
	this->x = x;
	this->y = y;
}

//min and max are in local coords
bool GB::Point::mouseDrag(const MouseEvent &event, double xMin, double xMax, double yMin, double yMax, juce::Rectangle<int> bounds) {
	if (selected) {
		x = startX + GB::Point::toInternalX(event.getDistanceFromDragStartX(),bounds);
		y = startY - 1 + GB::Point::toInternalY(event.getDistanceFromDragStartY(), bounds); //leave for now. bad code
		if (x < xMin) x = xMin;
		if (x > xMax) x = xMax;
		if (y < yMin) y = yMin;
		if (y > yMax) y = yMax; 
		return true;
	}
	return false;
}

void GB::Point::paint(Graphics& g, juce::Rectangle<int> bounds) {
	if (selected) g.setColour(juce::Colours::red);
	else g.setColour(juce::Colours::black);
	g.fillEllipse(GB::Point::toExternalX(x,bounds) - radius,
		GB::Point::toExternalY(y, bounds) - radius,
		radius * 2, radius * 2);
}

double GB::Point::toInternalX(int x, juce::Rectangle<int> bounds) {
	double width = bounds.getWidth(); //implicit cast to double
	return juce::jmap(double(x), 0.0, width, 0.0, 1.0);
}

double GB::Point::toInternalY(int y, juce::Rectangle<int> bounds) {
	double height = bounds.getHeight();
	return juce::jmap(double(y), height, 0.0, 0.0, 1.0);
}

int GB::Point::toExternalX(double x, juce::Rectangle<int> bounds) {
	//assumption that x is between 0 and 1
	return juce::jmap(x, 0.0, double(bounds.getWidth())-.00000000001);
}

int GB::Point::toExternalY(double y, juce::Rectangle<int> bounds) {
	//assumption that y is between 0 and 1
	return juce::jmap(y, double(bounds.getHeight())-.00000000001, 0.0);
}

String GB::Point::toString() {
	return "";
}