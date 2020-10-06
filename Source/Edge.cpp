/*
  ==============================================================================

    Edge.cpp
    Created: 2 Sep 2020 1:05:07pm
    Author:  gabey

  ==============================================================================
*/

#include "Edge.h"
Edge::Edge(double x, double y, double skew) : start(x, y, 8), skewPoint(0, 0, 5), resolution(20), skewData(resolution + 1)
{
	setSkew(skew);
	next = nullptr;
	prev = nullptr;
}

Edge::Edge(double x, double y) : Edge(x, y, 0.5) {}

//if data is out of range, returns -1
double Edge::readData(double t) {
	if (next == nullptr) return -1;
	if (t < start.x || t > next->start.x) {
		return -1;
	}
	double index = juce::jmap(double(t), double(start.x), double(next->start.x), 0.0, double(resolution));
	double low = skewData[(int)index];
	double high = 0.0;
	if (index < resolution) high = skewData[(int)index + 1];
	double r = index - (int)index;
	double linearInt = low * (1 - r) + high * r;
	double y1 = start.y;
	double y2 = next->start.y;
	double ret = juce::jmap(linearInt, 0.0, 1.0, y1, y2);
	return ret;
}

void Edge::setSkewPoint() {
	if (next != nullptr) {
		double x = (start.x + next->start.x) / 2.0;
		double y = start.y * (1.0 - skew) + next->start.y * skew;
		skewPoint.x = x;
		skewPoint.y = y;
	}
	if (prev != nullptr) {
		double x = (start.x + prev->start.x) / 2.0;
		double y = prev->start.y * (1.0 - prev->skew) + start.y * prev->skew;
		prev->skewPoint.x = x;
		prev->skewPoint.y = y;
	}
}

void Edge::setSkew(double newSkew) {
	if (newSkew >= 1.0) skew = .99999999999;
	else if (newSkew <= 0.0) skew = .00000000001;
	else if (newSkew == 0.5) skew = .50000000001;
	else skew = newSkew;
	double b = (1 - skew) / skew;
	double a = skew / (b - 1.0);
	double c = -a;
	for (int i = 0; i < resolution + 1; ++i) {
		double t = double(i) / (double(resolution) / 2.0); //t goes from 0 to 2
		skewData[i] = a * std::pow(b, t) + c;
	}
}

Edge* Edge::getNext() {
	return next;
}

Edge* Edge::getPrev() {
	return prev;
}

void Edge::setNext(Edge *n) {
	next = n;
	setSkewPoint();
}

void Edge::setPrev(Edge *p) {
	prev = p;
}

bool Edge::mouseDown(const MouseEvent &event, juce::Rectangle<int> bounds) {
	start.mouseDown(event, bounds);
	skewPoint.mouseDown(event, bounds);
	return false;
}

void Edge::moveStart(double x, double y) {
	start.movePoint(x, y);
	setSkewPoint();
}

bool Edge::mouseDrag(const MouseEvent &event, juce::Rectangle<int> bounds) {
	if (next == nullptr || prev == nullptr) {
		if (start.mouseDrag(event, start.x, start.x, 0, GB::Point::toInternalY(0.0,bounds), bounds)) {
			setSkewPoint();
			return true;
		}
	} else if (start.mouseDrag(event, prev->start.x, next->start.x, 0, GB::Point::toInternalY(0.0, bounds), bounds)) {
		setSkewPoint();
		return true;
	}
	if (next != nullptr) {
		auto low = next->start.y;
		auto high = start.y;
		if (start.y <= next->start.y) {
			low = start.y;
			high = next->start.y;
		}
		if (skewPoint.mouseDrag(event, skewPoint.x, skewPoint.x, low, high, bounds)) {
			double newSkew = double(start.y - skewPoint.y) / double(start.y - next->start.y);
			setSkew(newSkew);
			return true;
		}
	}
	return false;
}

void Edge::paintEdge(Graphics& g, juce::Rectangle<int> bounds) {
	double x1 = GB::Point::toExternalX(start.x, bounds);
	double y1 = GB::Point::toExternalY(start.y, bounds);
	double x2 = GB::Point::toExternalX(next->start.x, bounds);
	double y2 = GB::Point::toExternalY(next->start.y, bounds);
	double xp = (x1 + x2) / 2;
	double yp = y2 * skew + y1 * (1.0 - skew);

	//result = (a*pow(b, (t - x1)*x2 / 2) + c + y1)*y2;
	for (int i = 0; i < resolution; ++i) {
		g.drawLine((float)juce::jmap(double(i), 0.0, double(resolution), x1, x2),
			(float)juce::jmap(skewData[i], 0.0, 1.0, y1, y2),
			(float)juce::jmap(double(i) + 1.0, 0.0, double(resolution), x1, x2),
			(float)juce::jmap(skewData[i + 1], 0.0, 1.0, y1, y2));
	}
}

void Edge::paint(Graphics& g, juce::Rectangle<int> bounds) {
	g.setColour(juce::Colours::black);
	if (next != nullptr) {
		paintEdge(g, bounds);
		skewPoint.paint(g, bounds);
	}
	start.paint(g, bounds);
}

void Edge::backgroundPaint(Graphics& g, juce::Rectangle<int> bounds) {
	g.setColour(juce::Colours::white);
	if (next != nullptr) {
		paintEdge(g, bounds);
	}
}

String Edge::toString() {
	String s = "";
	s += juce::String("1.0 ");
	s += juce::String(std::to_string(start.x) + " " + std::to_string(start.y));
	s += juce::String(" " + std::to_string(skew));
	return s;
}