/*
  ==============================================================================

    Envelope.cpp
    Created: 2 Sep 2020 12:39:13pm
    Author:  gabey

  ==============================================================================
*/

#include "Envelope.h"

Envelope::Envelope() : Envelope(440.0) {}

Envelope::Envelope(double freq) {
	head = new Edge(0.0, 0.0);
	tail = new Edge(1.0, 0.0);
	frequency = freq;
	head->setNext(tail);
	head->setPrev(nullptr);
	tail->setPrev(head);
	tail->setNext(nullptr);
	current = head;
	time = 0.0;
	current_sample = 0.0;
	length = 1.0;
	sampleRate = 0.0;
}

Envelope::~Envelope() {
	Edge *e = head;
	while (head != nullptr) {
		e = head;
		head = head->getNext();
		delete e;
	}
}

void Envelope::setSampleRate(double s) {
	sampleRate = s;
}

double Envelope::getNext() {
	double d = current->readData(time);
	int i = 0;
	while (d < 0.0) {
		if (++i > 1000) return 0.0; //bad stopgap to keep from infinite loop;
		current = current->getNext();
		if (current == nullptr) {
			current = head;
		}
		d = current->readData(time);
	}
	time += 1.0 / sampleRate;
	while (time >= length) time -= length;
	while (time < 0.0) time += length;
	current_sample = d;
	return d;
}


void Envelope::addData(double x, double y, double skew) {
	Edge *current = head;
	Edge *previous = nullptr;
	while (current != nullptr) { //find where to put the new point
		if (current->start.x == x) break;
		if (current->start.x > x && previous->start.x <= x) break;
		previous = current;
		current = current->getNext();
	}
	if (current->start.x == x) {
		current->start.movePoint(x, y);
	}else if (previous != nullptr) { //if there are currently points
		Edge *newE = new Edge(x,y,skew);
		previous->setNext(newE);
		newE->setPrev(previous);
		newE->setNext(current);
		current->setPrev(newE);
		//tail = previous->next;
	}
	else {
		head = new Edge(x,y,skew);
		tail = head;
	}
}

void Envelope::addData(double x, double y) {
	addData(x, y, 0.5);
}

//double click makes a new point
void Envelope::mouseDoubleClick(const MouseEvent &event, juce::Rectangle<int> bounds) {
	Edge *current = head;
	Edge *previous = nullptr;
	auto clickX = GB::Point::toInternalX(event.getMouseDownX(), bounds);
	while (current != nullptr) { //find where to put the new point
		if (current->start.x >= clickX && previous->start.x <= clickX) break;
		previous = current;
		current = current->getNext();
	}
	if (previous != nullptr) { //if there are currently points
		Edge *newE = new Edge(GB::Point::toInternalX(event.getMouseDownX(), bounds),
			GB::Point::toInternalY(event.getMouseDownY(), bounds));
		previous->setNext(newE);
		newE->setPrev(previous);
		newE->setNext(current);
		current->setPrev(newE);
		//tail = previous->next;
	}
	else {
		head = new Edge(GB::Point::toInternalX(event.getMouseDownX(), bounds),
			GB::Point::toInternalY(event.getMouseDownY(), bounds));
		tail = head;
	}
}

void Envelope::mouseDown(const MouseEvent &event, juce::Rectangle<int> bounds) {
	Edge *c = head;
	bool oneSelected = false;//pr
	while (c != nullptr) {
		if (c->mouseDown(event, bounds)) break;
		c = c->getNext();
	}
}

void Envelope::mouseDrag(const MouseEvent &event, juce::Rectangle<int> bounds) {
	Edge *c = head;
	while (c != nullptr) {
		auto width = bounds.getWidth();
		auto height = bounds.getHeight();
		if (c->mouseDrag(event, bounds)) break;
		c = c->getNext();
	}
}

void Envelope::mouseUp(const MouseEvent &event, juce::Rectangle<int> bounds) {
	Edge *current = head;
	while (current != nullptr) {
		current->start.startX = current->start.x;
		current->start.startY = current->start.y;
		current = current->getNext();
	}
}

bool Envelope::keyPressed(const KeyPress &event, juce::Rectangle<int> bounds) {
	if (event.isKeyCode(KeyPress::backspaceKey) || event.isKeyCode(KeyPress::deleteKey)) {
		Edge *current = head;
		Edge *previous = nullptr;
		while (current != nullptr) {
			if (current->start.selected && current != head && current != tail) {
				//delete current
				if (previous == nullptr) {
					head = current->getNext();
					delete current;
					current = head;
					if (head != nullptr) head->setPrev(nullptr);
				}
				else {
					previous->setNext(current->getNext());
					if (current->getNext() != nullptr) {
						current->getNext()->setPrev(previous);
					}
					else {
						tail = previous;
					}
					delete current;
					current = previous->getNext();
				}
			}
			else {
				previous = current;
				current = current->getNext();
			}
		}
		return true;
	}
	return false;
}

void Envelope::paint(Graphics& g, juce::Rectangle<int> bounds){
	Edge *c = head;
	while (c != nullptr) {
		c->paint(g, bounds);
		c = c->getNext();
	}
}

void Envelope::backgroundPaint(Graphics& g, juce::Rectangle<int> bounds) {
	Edge *c = head;
	while (c != nullptr) {
		c->backgroundPaint(g, bounds);
		c = c->getNext();
	}
}

String Envelope::toString() {
	String s = "";
	s += juce::String("sin " + std::to_string(frequency) + "\n");
	Edge *e = head;
	while (e != nullptr) {
		s += juce::String(e->toString()+"\n");
		e = e->getNext();
	}
	return s;
}