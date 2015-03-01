#include <RGBPatterns.h>

int updateDelay = 1500;

RGBPatterns rgbPatterns( 3, 5, 6, updateDelay);

void setup(){
	Serial.begin(115200);
	// change the default mode to rainbow
	rgbPatterns.setMode(MODE_RAINBOW);
}

void loop(){
	Serial.println("loop");
	rgbPatterns.update();
}