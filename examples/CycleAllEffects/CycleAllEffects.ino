#include <RGBEffects.h>

int redPin = 3;
int greenPin = 5;
int bluePin = 6;

int updateDelay = 200;

int updateCount = 0;

RGBEffects rgbEffects( redPin, greenPin, bluePin );

void setup(){
	Serial.begin(115200);

	randomSeed(analogRead(0));
}

void loop(){
	rgbEffects.update();

	updateCount++;

	if(updateCount > 100){
		rgbEffects.nextEffect();
		Serial.println("Changing effect.");
		updateCount = 0;
	}

	delay(updateDelay);
}