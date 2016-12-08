# PatioLightsClient
The client Arduino program for the Patio Lights project.

**See [PatioLightsHost](https://github.com/Chris-Johnston/PatioLightsHost) First!**

## Setup

Installing the client is easy. I used a knockoff Arduino Mega, because at one point in development I needed all the extra memory I could get.

Add [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) to your Arduino configurations libraries folder. *todo consider using fastled*

Upload `patioLightsController.ino` to your Arduino. This can be done in any platform, as long as it supports Arduino.

### Raspberry Pi + Arduino CLI Troubles

One difficulty I encountered was uploading the code to the Arduino from a raspberry pi.
I didn't want to have to run outside with a laptop every time I wanted to make changes to the code, 
so I had to find some solution. I had no luck getting Arduino's command line interface to work on a headless raspberry pi, however
I found that the command line application [Ino](http://inotool.org/) did just the trick.

Ino has some quirks with the way it is set up. Here is my solution:

1. Make a directory to store the Ino stuff. `mkdir clientCode`

2. Set up Ino. `ino init`

3. Create a makefile to copy files from PatioLightsClient into the src directory of Ino, then build and upload it. Adjust `ARDUINO_TYPE`
and `PORT` to match your configuration.

  `makefile`

  ```makefile
  ARDUINO_TYPE=mega2560
  PORT=/dev/ttyACM0
  
  all: clean build upload
  
  clean:
          ino clean
  copy:
          cp -R ../PatioLightsClient/patioLightsController.ino src/sketch.ino
          cp -R ../PatioLightsClient/Patterns.h src/Patterns.h
          cp -R ../PatioLightsClient/RGBColor.h src/RGBColor.h
          cp -R ../PatioLightsClient/RGBColor.cpp src/RGBColor.cpp
  build: copy
          ino build -m $(ARDUINO_TYPE)
  upload:
          ino upload -m $(ARDUINO_TYPE) -p $(PORT)
  ```
  
4. Upload to the Arduino using `make`.

5. Restart the Host program, either by rebooting the Pi, or starting the script manually.

### How I used NeoPixels and Serial.Read

Both the Adafruit_NeoPixel and FastLED libraries have the same issue with Neopixels. Due to their unique timing, drawing the colors to
them must be done with `noInterrupts()` (called internally). This is fine for normal use, but other operations like reading from Serial
depend on interrupts to function properly. When drawing a small number of leds this didn't cause many issues, but when scaled up to 
several hundred, it did. I noticed that strings sent over Serial were dropping bytes and were unreliable.

This is described in the [FastLED wiki](https://github.com/FastLED/FastLED/wiki/Interrupt-problems).

#### My ~~hack~~ solution:

To combat dropping of bytes, at the start of each loop the client checks the first character from the Serial buffer for the garbage
character `x`. If this character is found, the program will wait for the actual command to be sent over Serial. Once the entire 
command is received, I calculate a checksum and compare it to the command. If everything matches, the parameters of the pattern
generation are updated and the client resumes normal operation.

The host software will send duplicate commands only a few times before it starts to ignore them. I allow it to send the command more than
once to ensure that the command gets read by the client. I don't allow it to keep sending the same message forever, as the loop where the client
reads the actual command causes the leds to freeze for a brief moment.

It's not the best solution, but it gets the job done and goes around an unfixable problem.
