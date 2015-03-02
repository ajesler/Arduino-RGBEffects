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

class FadePattern : public Pattern {
	public:
		virtual void setup();
		virtual RGB update();
	private:
		int _targetColour;
		int _nIntensity;
		int _nDirection;
		static int _MAX_INTENSITY;
		static int _MIN_INTENSITY;
		static int _MAX_TARGETCOLOUR;

};

int FadePattern::_MAX_INTENSITY = 8;
int FadePattern::_MIN_INTENSITY = 0;
int FadePattern::_MAX_TARGETCOLOUR = 7;

void FadePattern::setup(){
	_targetColour = 1;
	_nIntensity = 0;
	_nDirection = 1;
}

RGB FadePattern::update(){
	if (DEBUG){ Serial.println("FadePattern::update"); }

	// Update the intensity value
	if (_nDirection) {
		// Direction is positive, fading towards the color
		if (++_nIntensity >= _MAX_INTENSITY) {
		  // Maximum intensity reached
		  _nIntensity = _MAX_INTENSITY;  // Just in case
		  _nDirection = 0;             // Now going to fade OUT
		} // else : nothing to do
	} else {
		if (--_nIntensity <= _MIN_INTENSITY) {
		  _nIntensity = _MIN_INTENSITY; // Just in case
		  // When we get back to black, find the next target color
		  if (++_targetColour>_MAX_TARGETCOLOUR) 
		    _targetColour=1;          // We'll skip fading in and out of black
		  _nDirection = 1;             // Now going to fade IN
		} // else: nothing to do
	}

	// Compute the colors
	int colours[3];
	for (int i=0;i<3;i++) {
		// If the corresponding bit in targetColor is set, it's part of the target color
		// wth is going on here?
		colours[i] = (_targetColour & (1<<i)) ? (1<<_nIntensity) -1 : 0;
	}

	RGB colour = { colours[0], colours[1], colours[2] };

	return colour;
}

class CubePattern : public Pattern {
	public:
		virtual void setup();
		virtual RGB update();
	private:
		int _dx, _dy, _dz;
		RGB _colour;
		int _ccStep;
		int _vi1, _vi2;
		static point _VERTEX[];
		static int _MAX_VERTICES;
		static int _STEP_MULT;
};

point CubePattern::_VERTEX [] = 
{
  { 0, 0, 0 },
  { 0, 0, 1 },
  { 0, 1, 1 },
  { 0, 1, 0 },
  { 1, 1, 0 },
  { 1, 0, 0 },
  { 1, 0, 1 },
  { 1, 1, 1 }
};

int CubePattern::_MAX_VERTICES = (sizeof(_VERTEX)/sizeof(point));

int CubePattern::_STEP_MULT = 10;

void CubePattern::setup(){
	_ccStep = 0;
	_dx, _dy, _dz = 0;

	_vi2 = random(_MAX_VERTICES);
	_colour.r = _VERTEX[_vi2].x*250;
	_colour.g = _VERTEX[_vi2].y*250;
	_colour.b = _VERTEX[_vi2].z*250;
}

RGB CubePattern::update(){

	if(DEBUG){ Serial.println("CubePattern::update"); }

	// if completed traversal, start a new one
	if(((_dx == 0) && (_dy == 0) && (_dz == 0)) || _ccStep > 245 ){
		_vi1 = _vi2;

		do {
			_vi2 = random(_MAX_VERTICES);
		} 
		while (_vi1 == _vi2);

		_ccStep = 0;

		_dx = (_VERTEX[_vi2].x-_VERTEX[_vi1].x)*_STEP_MULT;
		_dy = (_VERTEX[_vi2].y-_VERTEX[_vi1].y)*_STEP_MULT;
		_dz = (_VERTEX[_vi2].z-_VERTEX[_vi1].z)*_STEP_MULT;
	}

	// advance the traversal
	_ccStep += _STEP_MULT;
	_colour.r += _dx; 
	_colour.g += _dy; 
	_colour.b += _dz;

	return _colour;
}

/*
RGBPatterns class
*/
RGBPatterns::RGBPatterns(int redPin, int greenPin, int bluePin){
	_redPin = redPin;
	_greenPin = greenPin;
	_bluePin = bluePin;

	// initialize LED pins
	pinMode(_redPin, OUTPUT);
	pinMode(_greenPin, OUTPUT);
	pinMode(_bluePin, OUTPUT);

    SolidPattern *solid = new SolidPattern();
    solid->setup();
    _patterns[0] = solid;
    _solidPattern = solid;

    RainbowPattern *rainbow = new RainbowPattern();
    rainbow->setup();
    _patterns[1] = rainbow;

    FadePattern *fade = new FadePattern();
    fade->setup();
    _patterns[2] = fade;

    CubePattern *cube = new CubePattern();
    cube->setup();
    _patterns[3] = cube;

    _currentPattern = 0;
}

RGBPatterns::~RGBPatterns(){
	delete[] _patterns;
}

void RGBPatterns::setMode(RGBModes mode){
	if (DEBUG){ Serial.println("RGBPatterns::setMode"); }

	setLEDsColour(OFF);

	switch(mode){
		case MODE_SOLID_RED:
			_solidPattern->setColour(RED);
			_currentPattern = 0;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode SOLID_RED"); }
			break;
		case MODE_SOLID_GREEN:
			_solidPattern->setColour(GREEN);
			_currentPattern = 0;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode SOLID_GREEN"); }
			break;
		case MODE_SOLID_BLUE:
			_solidPattern->setColour(BLUE);
			_currentPattern = 0;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode SOLID_BLUE"); }
			break;
		case MODE_SOLID_YELLOW:
			_solidPattern->setColour(YELLOW);
			_currentPattern = 0;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode SOLID_YELLOW"); }
			break;
		case MODE_SOLID_PURPLE:
			_solidPattern->setColour(PURPLE);
			_currentPattern = 0;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode SOLID_PURPLE"); }
			break;
		case MODE_FADE_RANDOM:
			_currentPattern = 3;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode FADE"); }
			break;
		case MODE_RAINBOW:
			_currentPattern = 1;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode RAINBOW"); }
			break;
		case MODE_CUBE:
			_currentPattern = 2;
			if (DEBUG2){ Serial.println("RGBPatterns::setMode CUBE"); }
			break;
	}

	_curMode = mode;
}

void RGBPatterns::nextMode(){
	setLEDsColour(OFF);

	if((int)_curMode+1 >= MODE_COUNTER_FINAL){
		_curMode = (RGBModes)0;
	} else {
		_curMode = (RGBModes)(_curMode+1);
	}

	setMode(_curMode);
}

void RGBPatterns::update(){
    RGB colour = _patterns[_currentPattern]->update();
	setLEDsColour(colour);
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

