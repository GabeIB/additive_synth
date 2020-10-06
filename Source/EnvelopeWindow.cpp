/*
  ==============================================================================

    EnvelopeWindow.cpp
    Created: 2 Sep 2020 12:32:56pm
    Author:  gabey

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeWindow.h"
#include <fstream>
#include <string>
#include <iostream>

//==============================================================================
EnvelopeWindow::EnvelopeWindow(std::vector<Envelope*>& e)
{
	startTimer(60);
	setWantsKeyboardFocus(true);
	envPointer = &e;
}

EnvelopeWindow::~EnvelopeWindow()
{
}

void EnvelopeWindow::addEnvelope()
{
	envPointer->push_back(new Envelope());
}

void EnvelopeWindow::addEnvelope(double freq)
{
	envPointer->push_back(new Envelope(freq));
}

void EnvelopeWindow::addData(int index, double x, double y)
{
	if (index >= 0 && index < envPointer->size()) {
		(*envPointer)[index]->addData(x,y);
		repaint();
	}
}

void EnvelopeWindow::addData(int index, double x, double y, double skew)
{
	if (index >= 0 && index < envPointer->size()) {
		(*envPointer)[index]->addData(x, y, skew);
		repaint();
	}
}

void EnvelopeWindow::removeEnvelope(int index)
{
	Envelope *e = (*envPointer)[index];
	for (int i = index; i < envPointer->size() - 1; ++i) {
		(*envPointer)[i] = (*envPointer)[i + 1];
	}
	envPointer->pop_back();
	delete e;
}

void EnvelopeWindow::clear() {
	while (envPointer->size() > 0) {
		removeEnvelope(0);
	}
}

void EnvelopeWindow::setFrequency(int index, double f) {
	Envelope *e = (*envPointer)[index];
	e->frequency = f;
}

void EnvelopeWindow::timerCallback()
{
	//envelope->getNext();
	repaint();
}

void EnvelopeWindow::paint (Graphics& g)
{
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);

	auto bounds = getLocalBounds();
	for (int i = 0; i < envPointer->size(); ++i) {
		if (i == selected_envelope) {
			(*envPointer)[i]->paint(g, bounds);
		}
		else {
			(*envPointer)[i]->backgroundPaint(g, bounds);
		}
	}
	g.setColour(juce::Colours::red);
	//g.fillEllipse(GB::Point::toExternalX(envelope->time, bounds) -2,
	//	GB::Point::toExternalY(envelope->current_sample, bounds) -2, 4, 4);
	if (envPointer->size() > 0) {
		g.drawLine(GB::Point::toExternalX((*envPointer)[0]->time, bounds), 0,
			GB::Point::toExternalX((*envPointer)[0]->time, bounds), bounds.getHeight());
	}
}

void EnvelopeWindow::resized()
{
}

void EnvelopeWindow::mouseDown(const MouseEvent &event) {
	if (selected_envelope >= 0) {
		(*envPointer)[selected_envelope]->mouseDown(event, getLocalBounds());
		repaint();
	}
}

void EnvelopeWindow::mouseDoubleClick(const MouseEvent &event) {
	if (selected_envelope >= 0) {
		(*envPointer)[selected_envelope]->mouseDoubleClick(event, getLocalBounds());
		repaint();
	}
}

void EnvelopeWindow::mouseDrag(const MouseEvent &event) {
	if (selected_envelope >= 0) {
		(*envPointer)[selected_envelope]->mouseDrag(event, getLocalBounds());
		repaint();
	}
}

void EnvelopeWindow::mouseUp(const MouseEvent &event) {
	if (selected_envelope >= 0) {
		(*envPointer)[selected_envelope]->mouseUp(event, getLocalBounds());
		repaint();
	}
}

bool EnvelopeWindow::keyPressed(const KeyPress &event) {
	if (selected_envelope >= 0) {
		bool b = (*envPointer)[selected_envelope]->keyPressed(event, getLocalBounds());
		repaint();
		return b;
	}
	return false;
}

void EnvelopeWindow::saveParams(std::string url) {
	std::ofstream file;
	file.open(url);
	file << "version: 1.0" << std::endl;
	file << "base frequency: 440.0" << std::endl;
	file << "duration: 1.0" << std::endl;
	file << "number of oscillators: " + std::to_string(envPointer->size()) << std::endl;
	file << "{\n";
	for (int i = 0; i < envPointer->size(); ++i) {
		file << (*envPointer)[i]->toString() + ";\n";
	}
	file << "}\n";
	file.close();
}

std::vector<double> EnvelopeWindow::parseEdge(std::string s) {
	std::vector<double> r;
	std::istringstream edge(s);
	double version = 0.0;
	double buffer = 0.0;
	edge >> version;
	if (version - 1.0 < .0000001) {
		edge >> buffer;
		r.push_back(buffer);
		edge >> buffer;
		r.push_back(buffer);
		edge >> buffer;
		r.push_back(buffer);
	}
	return r;
}

void EnvelopeWindow::loadParams(std::string url) {
	std::ifstream file;
	file.open(url);
	std::string buffer;
	std::string oscType;
	std::string edge;
	double frequency;
	int voiceCount = 0;
	double version = 1.0;
	double duration = 1.0;
	double bassFreq = 440.0;
	bool validFile = true;
	if (validFile) {
		clear();
	}
	std::getline(file, buffer, '{');
	file.get(); //remove \n
	std::getline(file, buffer, ';');
	while (buffer != "") {
		std::istringstream voice(buffer);
		voice >> oscType;
		voice >> frequency;
		voice.get(); //remove \n
		if (oscType == "sin") {
			if (frequency > 0) {
				addEnvelope(frequency);
				std::getline(voice, edge);
				while (edge != "") {
					std::vector<double> point = parseEdge(edge);
					addData(envPointer->size() - 1, point[0], point[1], point[2]);
					std::getline(voice, edge);
				}
			}
		}
		else {
			break;
		}
		file.get(); //remove \n
		std::getline(file, buffer, ';');
	}

}