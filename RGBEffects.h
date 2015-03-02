/*
RGBEffects - Library for making colour pattern effects with RGB LEDs.
*/
#ifndef RGBEffects_h
#define RGBEffects_h

#include "Arduino.h"

#define DEBUG 1
#define DEBUG2 1

struct rgb {
  byte r, g, b;
};

struct point {
  byte x, y, z;
};

/* Colour definitions */
const rgb RED = { 255, 0, 0 };
const rgb GREEN = { 0, 255, 0 };
const rgb BLUE = { 0, 0, 255 };
const rgb ORANGE = { 255, 165, 0 };
const rgb YELLOW = { 255, 255, 0 };
const rgb PURPLE = { 148, 0, 211 };
const rgb INDIGO = { 75, 0, 130 };
const rgb VIOLET = { 238, 130, 238 };
const rgb OFF = { 0, 0, 0 };


enum RGBEffects {
	EFFECT_SOLID_RED = 0,
	EFFECT_SOLID_GREEN,
	EFFECT_SOLID_BLUE,
	EFFECT_SOLID_YELLOW,
	EFFECT_SOLID_PURPLE,
	EFFECT_RAINBOW,
	EFFECT_FADE,
	EFFECT_CUBE,
	EFFECT_COUNTER_FINAL // dont use this one, just for counting
};

void printColour(rgb colour);
void printColour(int red, int green, int blue);

class Pattern {
public:
	virtual void setup();
	virtual rgb update();
};

class SolidEffect;

class RGBEffects
{

public:
	RGBEffects(int redPin, int greenPin, int bluePin);
	~RGBEffects();
	void setEffect(RGBEffects effect);
	void nextEffect();
	void update();

private:
	int _redPin;
	int _greenPin;
	int _bluePin;
	int _numLeds;
	SolidEffect *_solidEffect; // this is ugly, but need to be able to change the colour
	Effect * _effects[4];
	int _currentEffectIndex;
	RGBEffects _curEffect;
	void setLEDsColour(int red, int green, int blue);
	void setLEDsColour(rgb colour);
};

#endif