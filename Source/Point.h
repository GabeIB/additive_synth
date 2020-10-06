/*
  ==============================================================================

    Point.h
    Created: 2 Sep 2020 12:39:55pm
    Author:  gabey

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

namespace GB {
	class Point {
	public:
		double x;
		double y;
		double startX;
		double startY;
		int radius;
		bool selected;
		void movePoint(double, double);
		Point(double x, double y);
		Point(double x, double y, int r);
		bool withinPoint(double clickX, double clickY, juce::Rectangle<int> bounds);
		bool mouseDown(const MouseEvent &event, juce::Rectangle<int> bounds);
		bool mouseDrag(const MouseEvent &event, double xMin, double xMax, double yMin, double yMax, juce::Rectangle<int> bounds);
		void paint(Graphics& g, juce::Rectangle<int>);

		String toString();

		static double toInternalX(int, juce::Rectangle<int>);
		static double toInternalY(int, juce::Rectangle<int>);
		static int toExternalX(double, juce::Rectangle<int>);
		static int toExternalY(double, juce::Rectangle<int>);
	};
}