#include <RGBEffects.h>

int redPin = 3;
int greenPin = 5;
int bluePin = 6;
int updateDelay = 500;

RGBEffects rgbEffects( redPin, greenPin, bluePin );

void setup(){
	randomSeed(analogRead(0));

	// set the mode to rainbow
	rgbEffects.setEffect(EFFECT_RAINBOW);
}

void loop(){
	// update the LED colours.
	rgbEffects.update();

	// pause between each colour update
	delay(updateDelay);
}