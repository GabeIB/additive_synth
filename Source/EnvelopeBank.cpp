/*
  ==============================================================================

    EnvelopeBank.cpp
    Created: 16 Sep 2020 3:53:31pm
    Author:  gabey

  ==============================================================================
*/

/*#include "EnvelopeBank.h"

void EnvelopeBank::addEnvelope() {
	addEnvelope(440.0);
}

void EnvelopeBank::addEnvelope(double freq) {
	bank.push_back(new Envelope(freq));
}

void EnvelopeBank::removeEnvelope(int index) {
	Envelope *e = bank[index];
	for (int i = index; i < bank.size() - 1; ++i) {
		bank[i] = bank[i + 1];
	}
	bank.pop_back();
	delete e;
}

int EnvelopeBank::size() {
	return bank.size();
}

void EnvelopeBank::setFrequency(int index, double freq) {
	bank[index]->frequency = freq;
}

void EnvelopeBank::clear() {
	while (bank.size() > 0) {
		removeEnvelope(0);
	}
}

void EnvelopeBank::addPoint(int index, double x, double y, double skew) {
	if (index >= 0 && index < bank.size()) {
		bank[index]->addData(x, y, skew);
	}
}

void EnvelopeBank::addPoint(int envelope, double x, double y) {
	addData(envelope, x, y, 0.5);
}

std::vector<std::vector<double>> EnvelopeBank::getPoints(int index) {}
std::vector<std::vector<double>> EnvelopeBank::getSkewPoints(int index) {}
std::vector<std::vector<double>> EnvelopeBank::getEdge(int index) {}
*/