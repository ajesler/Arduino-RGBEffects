#include <RGBEffects.h>

int redPin = 3;
int greenPin = 5;
int bluePin = 6;

int updateDelay = 200;

int repeatCount = 0;

RGBEffects rgbEffects( redPin, greenPin, bluePin );

void setup(){
	Serial.begin(115200);

	randomSeed(analogRead(0));
}

void loop(){
	rgbEffects.update();

	repeatCount++;

	if(repeatCount > 100){
		rgbEffects.nextEffect();
		Serial.println("nextEffect");
		repeatCount = 0;
	}

	delay(updateDelay);
}