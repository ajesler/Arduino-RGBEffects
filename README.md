# RGBEffects

A library for displaying colour effects using RGB LEDs.  
The main motivation for writing this was to learn how to write an Arduino library, and tidy up the code for a remote controlled LED lamp project.  

Please note I had no experience with C++ before this. Apologies if the source makes you weep. Pull requests gladly accepted.


## Installation

Assuming you are using the standard Arduino IDE, clone or download the project, then copy it into your Arduino libraries directory. For more information, see the [Arduino Libraries Guide](http://arduino.cc/en/Guide/Libraries).

When the library is installed properly, it will be available in the Sketch > Import library list. 


## Usage

RGBEffects is concerned with calculating the next colour in an effect and displaying that colour. It is up to you to control the frequency that the `update()` method is called. The usual way to do this is use Arduinos `delay(ms)` function.

Note that you do not need to set the red, green and blue pin modes, this will be done in the rgbEffects initialisation.

### Examples

```c
#include <RGBEffects.h>

int redPin = 3;
int greenPin = 5;
int bluePin = 6;

int updateCount = 0;

RGBEffects rgbEffects( redPin, greenPin, bluePin );

void setup(){
  Serial.begin(115200);
}

void loop(){
  rgbEffects.update();
  updateCount++;

  // calls update 100 times before changing to the next effect.
  if(updateCount >= 100){
    updateCount = 0;
    rgbEffects.nextEffect();
    Serial.println("Changing effect.");
  }

  // 200ms delay between each update call.
  delay(200);
}
```

```c
#include <RGBEffects.h>

const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

int updateDelay = 500;

RGBEffects rgbEffects( redPin, greenPin, bluePin );

void setup(){
  rgbEffects.setEffect(EFFECT_BLINK);
}

void loop(){
  rgbEffects.update();
  delay(updateDelay);
}
```

### API

The RGBEffects class has the following public API.

* `RGBEffects(int redPin, int greenPin, int bluePin)`  
  Class constructor. `redPin`, `greenPin`, and `bluePin` are the pin numbers for the LED attached to the Arduino. 
  The default effect is `EFFECT_RAINBOW`
* `nextEffect()`  
   Changes to the next mode, as defined by the ordering in `RGBEffects.h`. It will loop back to the first mode if called on the last mode.
* `setEffect(RGBEffectType effect)`  
  Set the effect mode to a specific effect. 
* `update()`  
  Calculates the next colour in the effect and updates the LED.


### Available Effects

* `EFFECT_OFF` Turns the LEDs off. 
* `EFFECT_SOLID_RED` Shows a solid red colour RGB(255,0,0)
* `EFFECT_SOLID_GREEN` Shows a solid green colour RGB(0,255,0)
* `EFFECT_SOLID_BLUE` Shows a solid blue colour RGB(0,0,255) 
* `EFFECT_SOLID_YELLOW` Shows a solid yellow colour RGB(255,255,0)
* `EFFECT_SOLID_PURPLE` Shows a solid purple colour RGB(148,0,211) 
* `EFFECT_SOLID_VIOLET` Shows a solid violet colour RGB(238,130,238)
* `EFFECT_SOLID_WHITE` Shows a solid white colour RGB(255,255,255);
* `EFFECT_RAINBOW` Shows the colours of the rainbow in sequence (red, orange, yellow, green, blue, indigo, violet).
* `EFFECT_FADE` Fades between colours.
* `EFFECT_CUBE` Transitions in steps between different colours defined by a cube. 
* `EFFECT_BLINK` Alternates between turning the LED off and showing blue.
  

## Adding more effects

Adding new effects is pretty simple. 

1. Increase the size of the _effects array in RGBEffect.h
2. Create an entry for your new effect in RGBEffectType in RGBEffects.h
3. Create a subclass of Effect in RGBEffects.cpp
4. Create an instance of that class in the RGBEffects constructor and add it to the _effects array in RGBEffects.cpp
5. Add a case for your effect in the RGBEffects::setEffect method in RGBEffects.cpp

There is a commit which adds a new effect called [Blink](https://github.com/ajesler/Arduino-RGBEffects/commit/f985f01e614ff6f0f3d7ecba7096ab1712977045).

If you want to share your effect, send a pull request with your changes.

### Debugging effects

If you want to see what colour the update call is setting, change the 0 to a 1 in the line `#define DEBUG_COLOURS_ENABLED (0)` at the top of RGBEffects.h.


## Backlog

I am not planning to make many more changes to the library as for my current projects it works fine. Possible future work includes:

* Refactor to reduce memory usage when running.
* Add ability to control multiple RGB LEDs. At the moment I get around this problem by having two LEDs on a single set of pins which seems to work just swell.