#include "Arduino.h"
#include "RGBEffects.h"

void printColour(rgb colour){
	Serial.print("Colour: ");
	Serial.print(colour.r);
	Serial.print(", "); 
	Serial.print(colour.g);
	Serial.print(", "); 
	Serial.println(colour.b); 
}

void printColour(int red, int green, int blue){
	Serial.print("Colour: ");
	Serial.print(red);
	Serial.print(", "); 
	Serial.print(green);
	Serial.print(", "); 
	Serial.println(blue); 
}

/*
SolidPattern
Displays a solid colour
*/
class SolidEffect : public Effect {
	public:
		virtual void setup();
		virtual rgb update();
		void setColour(rgb colour);
	private:
		rgb _colour;
};

void SolidEffect::setup(){
	setColour(BLUE);
}

rgb SolidEffect::update(){
	return _colour;
}

void SolidEffect::setColour(rgb colour){
	_colour = colour;
}

/*
RainbowPattern
Displays the colours of the rainbow in sequence.
*/
class RainbowEffect : public Effect {
	public:
		virtual void setup();
		virtual rgb update();
	private:
		rgb _colours[7];
		int _curColour;
};

void RainbowEffect::setup(){
	_colours[0] = RED;
	_colours[1] = ORANGE;
	_colours[2] = YELLOW;
	_colours[3] = GREEN;
	_colours[4] = BLUE;
	_colours[5] = INDIGO;
	_colours[6] = VIOLET;
	_curColour = -1;
}

rgb RainbowEffect::update(){
	if(++_curColour > 6) { _curColour = 0; }
	return _colours[_curColour];
}

/*
FadePattern
Based on http://stackoverflow.com/a/15816597
Fades into a colour then off through a sequence
*/
class FadeEffect : public Effect {
	public:
		virtual void setup();
		virtual rgb update();
	private:
		int _targetColour;
		int _nIntensity;
		int _nDirection;
		static int _MAX_INTENSITY;
		static int _MIN_INTENSITY;
		static int _MAX_TARGETCOLOUR;

};

int FadeEffect::_MAX_INTENSITY = 8;
int FadeEffect::_MIN_INTENSITY = 0;
int FadeEffect::_MAX_TARGETCOLOUR = 7;

void FadeEffect::setup(){
	_targetColour = 1;
	_nIntensity = 0;
	_nDirection = 1;
}

rgb FadeEffect::update(){

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

	rgb colour = { colours[0], colours[1], colours[2] };

	return colour;
}

/*
CubePattern
Based on http://forum.arduino.cc/index.php?topic=102040.0
*/
class CubeEffect : public Effect {
	public:
		virtual void setup();
		virtual rgb update();
	private:
		int _dx, _dy, _dz;
		rgb _colour;
		int _ccStep;
		int _vi1, _vi2;
		static point _VERTEX[];
		static int _MAX_VERTICES;
		static int _STEP_MULT;
};

point CubeEffect::_VERTEX [] = 
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

int CubeEffect::_MAX_VERTICES = (sizeof(_VERTEX)/sizeof(point));

int CubeEffect::_STEP_MULT = 10;

void CubeEffect::setup(){
	_ccStep = 0;
	_dx, _dy, _dz = 0;

	_vi2 = random(_MAX_VERTICES);
	_colour.r = _VERTEX[_vi2].x*250;
	_colour.g = _VERTEX[_vi2].y*250;
	_colour.b = _VERTEX[_vi2].z*250;
}

rgb CubeEffect::update(){

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

class BlinkEffect : public Effect {
	public:
		virtual void setup();
		virtual rgb update();
	private:
		byte _blinkOn;
};

void BlinkEffect::setup(){
	_blinkOn = 1;
}

rgb BlinkEffect::update(){
	rgb c = (_blinkOn) ? BLUE : OFF;
	_blinkOn = !_blinkOn;
	return c;
}

/*
RGBEffects class
*/
RGBEffects::RGBEffects(int redPin, int greenPin, int bluePin){
	_redPin = redPin;
	_greenPin = greenPin;
	_bluePin = bluePin;

	// initialize LED pins
	pinMode(_redPin, OUTPUT);
	pinMode(_greenPin, OUTPUT);
	pinMode(_bluePin, OUTPUT);

  SolidEffect *solid = new SolidEffect();
  solid->setup();
  _effects[0] = solid;
  _solidEffect = solid;

  RainbowEffect *rainbow = new RainbowEffect();
  rainbow->setup();
  _effects[1] = rainbow;

  FadeEffect *fade = new FadeEffect();
  fade->setup();
  _effects[2] = fade;

  CubeEffect *cube = new CubeEffect();
  cube->setup();
  _effects[3] = cube;

  BlinkEffect *blink = new BlinkEffect();
  blink->setup();
  _effects[4] = blink;

  _currentEffectIndex = 1;
}

RGBEffects::~RGBEffects(){
	delete[] _effects;
}

void RGBEffects::setEffect(RGBEffectType effect){

	switch(effect){
		case EFFECT_SOLID_RED:
			_solidEffect->setColour(RED);
			_currentEffectIndex = 0;
			break;
		case EFFECT_SOLID_GREEN:
			_solidEffect->setColour(GREEN);
			_currentEffectIndex = 0;
			break;
		case EFFECT_SOLID_BLUE:
			_solidEffect->setColour(BLUE);
			_currentEffectIndex = 0;
			break;
		case EFFECT_SOLID_YELLOW:
			_solidEffect->setColour(YELLOW);
			_currentEffectIndex = 0;
			break;
		case EFFECT_SOLID_PURPLE:
			_solidEffect->setColour(PURPLE);
			_currentEffectIndex = 0;
			break;
		case EFFECT_SOLID_VIOLET:
			_solidEffect->setColour(VIOLET);
			_currentEffectIndex = 0;
			break;
		case EFFECT_SOLID_WHITE:
			_solidEffect->setColour(WHITE);
			_currentEffectIndex = 0;
			break;
		case EFFECT_RAINBOW:
			_currentEffectIndex = 1;
			break;
		case EFFECT_FADE:
			_currentEffectIndex = 2;
			break;
		case EFFECT_CUBE:
			_currentEffectIndex = 3;
			break;
		case EFFECT_BLINK:
			_currentEffectIndex = 4;
			break;
	}

	_curEffect = effect;
}

void RGBEffects::nextEffect(){
	setLEDsColour(OFF);

	if((int)_curEffect+1 >= EFFECT_COUNTER_FINAL){
		_curEffect = (RGBEffectType)0;
	} else {
		_curEffect = (RGBEffectType)(_curEffect+1);
	}

	setEffect(_curEffect);
}

void RGBEffects::update(){
  rgb colour = _effects[_currentEffectIndex]->update();
	setLEDsColour(colour);
}

void RGBEffects::setLEDsColour(int red, int green, int blue){
	if (RGB_DEBUG){ printColour(red, green, blue); }
	analogWrite(_redPin, red);
	analogWrite(_greenPin, green);
	analogWrite(_bluePin, blue);
}

void RGBEffects::setLEDsColour(rgb colour){
	if (RGB_DEBUG){ printColour(colour); }
	analogWrite(_redPin, colour.r);
	analogWrite(_greenPin, colour.g);
	analogWrite(_bluePin, colour.b);
}