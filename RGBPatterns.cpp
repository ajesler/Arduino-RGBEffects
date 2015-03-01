#include "Arduino.h"
#include "RGBPatterns.h"

void printColour(RGB colour){
	Serial.print("Colour: ");
	Serial.print(colour.r);
	Serial.print(", "); 
	Serial.print(colour.g);
	Serial.print(", "); 
	Serial.println(colour.b); 
}

/*
PATTERNS
*/
class SolidPattern : public Pattern {
	public:
		virtual void setup();
		virtual RGB update();
		void setColour(RGB colour);
	private:
		RGB _colour;
};

void SolidPattern::setup(){
	if (DEBUG){ Serial.println("SolidPattern::setup"); }
	setColour(GREEN);
}

RGB SolidPattern::update(){
	if (DEBUG){ Serial.println("SolidPattern::update"); }
	return _colour;
}

void SolidPattern::setColour(RGB colour){
	_colour = colour;
}

/*
RGBPatterns class
*/
RGBPatterns::RGBPatterns(int redPin, int greenPin, int bluePin, int updateDelay){
	_redPin = redPin;
	_greenPin = greenPin;
	_bluePin = bluePin;
	_updateDelay = updateDelay;

	// initialize LED pins
	pinMode(_redPin, OUTPUT);
	pinMode(_greenPin, OUTPUT);
	pinMode(_bluePin, OUTPUT);

    SolidPattern solidPattern;
    solidPattern.setup();
    _solid = &solidPattern;
    _patterns[0] = &solidPattern;
    _currentPattern = 0;
}

RGBPatterns::~RGBPatterns(){
	delete[] _patterns;
}

void RGBPatterns::setMode(RGBModes mode){
	if (DEBUG){ Serial.println("RGBPatterns::setMode"); }
}

void RGBPatterns::nextMode(){

}

int RGBPatterns::getUpdateDelay(){
	return _updateDelay;
}

void RGBPatterns::setUpdateDelay(int updateDelay){
	_updateDelay = updateDelay;
}

void RGBPatterns::update(){
	// Pattern *p = _patterns[_currentPattern];
	// RGB newColour = p->update();
	// _patterns[0]->update();
	SolidPattern solidPattern;
    solidPattern.setup();
    RGB colour = _solid.update();
	setLEDsColour(colour);
	delay(_updateDelay);
}

void RGBPatterns::setLEDsColour(int red, int green, int blue){
	analogWrite(_redPin, red);
	analogWrite(_greenPin, green);
	analogWrite(_bluePin, blue);
}

void RGBPatterns::setLEDsColour(RGB colour){
	if (DEBUG){ printColour(colour) }
	analogWrite(_redPin, colour.r);
	analogWrite(_greenPin, colour.g);
	analogWrite(_bluePin, colour.b);
}

