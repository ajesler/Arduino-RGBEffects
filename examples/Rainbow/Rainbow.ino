#include <RGBPatterns.h>

int redPin = 3;
int greenPin = 5;
int bluePin = 6;
int updateDelay = 200;

int repeatCount = 0;

RGBPatterns rgbPatterns( redPin, greenPin, bluePin );

void setup(){
	Serial.begin(115200);

	randomSeed(analogRead(0));

	// change the default mode to rainbow
	rgbPatterns.setMode(MODE_RAINBOW);
}

void loop(){
	rgbPatterns.update();

	repeatCount++;

	if(repeatCount > 100){
		rgbPatterns.nextMode();
		Serial.println("nextMode");
		repeatCount = 0;
	}

	delay(updateDelay);
}