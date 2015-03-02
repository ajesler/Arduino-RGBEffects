# RGBEffects

A library for displaying colour effects using RGB LEDs. 

## Usage

RGBEffects is concerned with calculating the next colour in an effect and setting the LED pins. 

### Example Usage

```c
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
```

### API

The RGBEffects class has the following public API.

* `RGBEffects(int redPin, int greenPin, int bluePin)`  
  Class constructor. `redPin`, `greenPin`, and `bluePin` are the pin numbers for the LED attached to the Arduino. 
  The default mode is `MODE_RAINBOW`
* `nextMode()`  
   Changes to the next mode, as defined by the ordering in `RGBEffects.h`. It will loop back to the first mode if called on the last mode.
* `setMode()`  
  Set the effect mode to a specific effect. 
* `update()`  
  Calculates the next colour in the effect and updates the LED.

### Available Modes

* `EFFECT_SOLID_RED`  
  Shows a solid red colour RGB(255,0,0)
* `EFFECT_SOLID_GREEN`  
  Shows a solid green colour RGB(0,255,0)
* `EFFECT_SOLID_BLUE`  
  Shows a solid blue colour RGB(0,0,255) 
* `EFFECT_SOLID_YELLOW`  
  Shows a solid yellow colour RGB(255,255,0)
* `EFFECT_SOLID_PURPLE` 
  Shows a solid purple colour RGB(148,0,211) 
* `EFFECT_RAINBOW`  
  Shows the colours of the rainbow in sequence (red, orange, yellow, green, blue, indigo, violet).
* `EFFECT_FADE`  
  Fades between colours.
* `EFFECT_CUBE`  
  Transitions in steps between different colours defined by a cube. 
  
### Adding more effects



Please note, I have no experience with C++. Apologies if the source makes you weep.