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
	setColour(BLUE);
}

RGB SolidPattern::update(){
	if (DEBUG){ Serial.println("SolidPattern::update"); }
	return _colour;
}

void SolidPattern::setColour(RGB colour){
	_colour = colour;
}

class RainbowPattern : public Pattern {
	public:
		virtual void setup();
		virtual RGB update();
	private:
		RGB _colours[7];
		int _curColour;
};

void RainbowPattern::setup(){
	if (DEBUG){ Serial.println("RainbowPattern::setup"); }
	_colours[0] = RED;
	_colours[1] = ORANGE;
	_colours[2] = YELLOW;
	_colours[3] = GREEN;
	_colours[4] = BLUE;
	_colours[5] = INDIGO;
	_colours[6] = VIOLET;
	_curColour = -1;
}

RGB RainbowPattern::update(){
	if (DEBUG){ Serial.println("RainbowPattern::update"); }
	if(++_curColour > 6) { _curColour = 0; }
	return _colours[_curColour];
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

    SolidPattern *solid = new SolidPattern();
    solid->setup();
    _patterns[0] = solid;

    RainbowPattern *rainbow = new RainbowPattern();
    rainbow->setup();
    _patterns[1] = rainbow;

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
    RGB colour = _patterns[1]->update();
	setLEDsColour(colour);
	delay(_updateDelay);
}

void RGBPatterns::setLEDsColour(int red, int green, int blue){
	analogWrite(_redPin, red);
	analogWrite(_greenPin, green);
	analogWrite(_bluePin, blue);
}

void RGBPatterns::setLEDsColour(RGB colour){
	if (DEBUG){ printColour(colour); }
	analogWrite(_redPin, colour.r);
	analogWrite(_greenPin, colour.g);
	analogWrite(_bluePin, colour.b);
}

