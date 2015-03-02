/*
RGBPatterns - Library for making colour pattern effects with RGB LEDs.
*/
#ifndef RGBPatterns_h
#define RGBPatterns_h

#include "Arduino.h"

#define DEBUG 0
#define DEBUG2 1

struct RGB {
  int r, g, b;
};

struct point {
  int x, y, z;
};

const RGB RED = { 255, 0, 0 };
const RGB GREEN = { 0, 255, 0 };
const RGB BLUE = { 0, 0, 255 };
const RGB ORANGE = { 255, 165, 0 };
const RGB YELLOW = { 255, 255, 0 };
const RGB PURPLE = { 148, 0, 211 };
const RGB INDIGO = { 75, 0, 130 };
const RGB VIOLET = { 238, 130, 238 };
const RGB OFF = { 0, 0, 0 };

enum RGBModes {
	MODE_SOLID_RED = 0,
	MODE_SOLID_GREEN,
	MODE_SOLID_BLUE,
	MODE_SOLID_YELLOW,
	MODE_SOLID_PURPLE,
	MODE_FADE_RANDOM,
	MODE_RAINBOW,
	MODE_CUBE,
	MODE_COUNTER_FINAL // dont use this one, just for counting
};

void printColour(RGB colour);

class Pattern {
public:
	virtual void setup();
	virtual RGB update();
};

class SolidPattern;

class RGBPatterns
{

public:
	RGBPatterns(int redPin, int greenPin, int bluePin);
	~RGBPatterns();

	void setMode(RGBModes mode);
	void nextMode();

	void update();

private:
	int _redPin;
	int _greenPin;
	int _bluePin;
	int _numLeds;
	SolidPattern *_solidPattern;
	Pattern * _patterns[4];
	int _currentPattern;
	RGBModes _curMode;
	void setLEDsColour(int red, int green, int blue);
	void setLEDsColour(RGB colour);
};

#endif