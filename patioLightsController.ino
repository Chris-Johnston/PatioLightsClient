#include <Adafruit_NeoPixel.h>
#include "Patterns.h"
#include "RGBColor.h"
#include <stdlib.h>

#define NUMBER_OF_STRIPS 4 // 1 for testing

// Configuration
#define BRIGHTNESS 100
#define BAUD_RATE 115200

struct StripData {
	bool isFront; // determines if the strip uses the front data
	int startIndex; // the starting index of the strips color data
	int length; // the length of the strip
	bool mirrored; // is the strip mirrored? normally pin # increments from left to right
	int pin; // the pin that the strip is connected to
};

// front left strand (mirrored)
StripData strips[NUMBER_OF_STRIPS] = {
	{ true, 0, 128, true, 7 },
	{ true, 128, 119, false, 6 },
	{ true, 0, 150, true, 8},
	{ true, 150, 210, false, 9}

	/*{ true, 0, 128, true, 7 },
	{ true, 128, 119, false, 6 },*/
	/*{ false, 0, 15, true, 8 },
	{ false, 15, 15, false, 9 }*/
};
// { true, 0, 128, true, D6 },
// { true, 128, 119, false, D7 }

//#define LED_COUNT_FRONT 247
#define LED_COUNT_FRONT 360
#define LED_COUNT_BACK 360
//#define LED_COUNT_FRONT 247
//#define LED_COUNT_BACK 360

Adafruit_NeoPixel NeoPixels[NUMBER_OF_STRIPS];
//= Adafruit_NeoPixel(STRIP1_LENGTH, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

RGBColor frontColors[LED_COUNT_FRONT];
RGBColor backColors[LED_COUNT_BACK];

char pattern;
char action;
RGBColor color1;
RGBColor color2;
double delayAnimationSpeed;
double delayHold;
//unsigned long actionTimer;
bool actionRunning;

String serialBuffer = "";

void setup()
{
	Serial.begin(BAUD_RATE);
	Serial.println("Start");
	// initialize the strips
	for (int i = 0; i < NUMBER_OF_STRIPS; i++)
	{
		NeoPixels[i] = Adafruit_NeoPixel(strips[i].length, strips[i].pin, NEO_GRB + NEO_KHZ800);
		NeoPixels[i].setBrightness(BRIGHTNESS);
		NeoPixels[i].begin();
	}

	//pattern = PATTERN_SCROLL;
	pattern = PATTERN_SCROLL;
	color1 = RGBColor(255,0,0); //255 185 0
	color2 = RGBColor(0, 0, 0); // 255 00 255
	delayAnimationSpeed = 150;
	delayHold = 0;
}

void loop()
{
	delay(5);
	// read any data from serial
	if (Serial.available())
	{
		serialBuffer += (char)Serial.read();
	}
	//Serial.println(serialBuffer);
	//String data = Serial.readStringUntil((char)END_OF_MESSAGE);
	//Serial.println(data);
	// ensure that the string has a start and an end, and that the start is before the end
	int indexStart = serialBuffer.indexOf(START_OF_MESSAGE);
	int indexEnd = serialBuffer.indexOf(END_OF_MESSAGE);
	int stripNumToChange = 0;

	if (indexStart != -1 &&
		indexEnd != -1 &&
		indexStart < indexEnd)
	{
		// message contains all of the data
		String message = serialBuffer.substring(indexStart + String(START_OF_MESSAGE).length(), indexEnd);
		serialBuffer = serialBuffer.substring(indexEnd, serialBuffer.length() - 1);
		Serial.flush();
		//Serial.println(message);
		// parse data
		stripNumToChange = ((int)message.charAt(0));
		//Serial.print("Pattern");
		pattern = message.charAt(1);
		action = pattern;
		//Serial.println(pattern);
		int r = message.substring(2, 5).toInt();
		int g = message.substring(5, 8).toInt();
		int b = message.substring(8, 11).toInt();
		int r2 = message.substring(11, 14).toInt();
		int g2 = message.substring(14, 17).toInt();
		int b2 = message.substring(17, 20).toInt();
		/*Serial.println(r);
		Serial.println(g);
		Serial.println(b);
		Serial.println(r2);
		Serial.println(g2);
		Serial.println(b2);*/
		color1 = RGBColor(r, g, b);
		color2 = RGBColor(r2, g2, b2);
		delayAnimationSpeed = message.substring(20, 24).toInt();
		delayHold = message.substring(24, 28).toInt();
		/*Serial.println(delayAnimationSpeed);
		Serial.println(delayHold);*/
		// message format
		// START front=1back=0 Pattern Color1 Color2(RGB) animationSpeed(millis 4 digits) animationSpeed(millis) ENDs
	}

	if (stripNumToChange == 1)
	{
			
	}
	else
	{
		// set parameters of 
		//strips[stripNumToChange - 1].
	}
	//Serial.flush();

	// iterate through strips and do their stuff
	// do patterns
	switch (pattern)
	{
	case PATTERN_SOLIDCOLOR:
		pattern_solidColor();
		break;
	case PATTERN_BLINK:
		pattern_blink();
		break;
	case PATTERN_DECAY:
		//pattern_decay();
		break;
	case PATTERN_LARSON:
		pattern_larson();
		break;
	case PATTERN_PULSE:
		pattern_pulse();
		break;
	case PRESET_REDWHITEANDBLUE:
		preset_redWhiteBlue();
		break;
	case PATTERN_SCROLL:
		pattern_scroll();
		break;
	case PATTERN_SCROLLSMOOTH:
		pattern_scrollsmooth();
		break;
	case PATTERN_WIPELEFT:
		pattern_wipeLeft();
		break;
	case PATTERN_WIPERIGHT:
		pattern_wipeRight();
		break;
	case PATTERN_RANDBLINK:
		pattern_randBlink();
		break;
	default:
		//pattern_blink();
		break;
	}
	//
	//actionTimer = millis();
	//// do actions (these are going over the patterns)
	//switch (action)
	//{
	//case ACTION_FADECOLOR:
	//	action_fadeColor();
	//	break;
	//case ACTION_FLASH:
	//	action_flash();
	//	break;
	//case ACTION_SETCOLOR:
	//	action_setColor();
	//	break;
	//case ACTION_WIPECENTER:
	//	action_wipeCenter();
	//	break;
	//case ACTION_WIPELEFT:
	//	action_wipeLeft();
	//	break;
	//case ACTION_WIPERIGHT:
	//	action_wipeRight();
	//	break;
	//default:
	//	break;
	//}

	//// do presets (these just go over everything), special cases
	//switch (action)
	//{
	//case PRESET_REDWHITEANDBLUE:
	//	preset_redWhiteBlue();
	//	break;
	//default:
	//	break;
	//}
	//

	// update all strips at once, not as they are computed

	// update the strips
	for (int stripNumber = 0; stripNumber < NUMBER_OF_STRIPS && strips[stripNumber].isFront; stripNumber++)
	{
		/*if (strips[i].isFront)
		{
			NeoPixels[i].setPixelColor()
		}
		else
		{
			NeoPixels[i].setPixelColor()
		}*/
		int initial =strips[stripNumber].startIndex;
		int total = strips[stripNumber].length + strips[stripNumber].startIndex;
		/*Serial.print("Strip");
		Serial.print(strips[i].isFront);
		Serial.print(strips[i].length);
		Serial.print(strips[i].mirrored);
		Serial.print(strips[i].pin);
		Serial.print(strips[i].startIndex);
		Serial.println("");*/
		for (int c = 0; c < strips[stripNumber].length; c++)
		{
			if (strips[stripNumber].isFront)
			{
				if (!strips[stripNumber].mirrored)
				{
					int index = c + strips[stripNumber].startIndex;
					NeoPixels[stripNumber].setPixelColor(c, frontColors[index].getR(), frontColors[index].getG(), frontColors[index].getB());
				}
				else
				{
					int index = strips[stripNumber].length - c + strips[stripNumber].startIndex;
					NeoPixels[stripNumber].setPixelColor(c, frontColors[index].getR(), frontColors[index].getG(), frontColors[index].getB());
				}
			}
			else
			{
				if (!strips[stripNumber].mirrored)
				{
					int index = c + strips[stripNumber].startIndex;
					NeoPixels[stripNumber].setPixelColor(c, backColors[index].getR(), backColors[index].getG(), backColors[index].getB());
				}
				else
				{
					int index = strips[stripNumber].length - c + strips[stripNumber].startIndex;
					NeoPixels[stripNumber].setPixelColor(c, backColors[index].getR(), backColors[index].getG(), backColors[index].getB());
				}
			}
			
		}
		NeoPixels[stripNumber].show();
	}
	/*for (int i = STRIP1_STARTINDEX; i < STRIP1_LENGTH; i++)
	{
		strip1.setPixelColor(i, frontColors[i].getR(), frontColors[i].getG(), frontColors[i].getB());
	}
	strip1.show();*/
}


unsigned long previousBlink = 0;
bool blinkToggle = false;
// patterns
// for now these will update all of the strips, but later I might consider
// assigning functions to different strips or different sides (front, back, side)
void pattern_blink()
{
	if (millis() - previousBlink >= delayAnimationSpeed)
	{
		previousBlink = millis();
		blinkToggle = !blinkToggle;
	}
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		if (blinkToggle)
		{
			frontColors[i] = color1;
		}
		else
		{
			frontColors[i] = color2;
		}
	}

	for (int i = 0; i < LED_COUNT_BACK; i++)
	{
		if (blinkToggle)
		{
			backColors[i] = color1;
		}
		else
		{
			backColors[i] = color2;
		}
	}
}

void pattern_solidColor()
{
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		frontColors[i] = color1;
		//strip1.setPixelColor(i, color1.getR(), color1.getG(), color1.getB());
	}

	for (int i = 0; i < LED_COUNT_BACK; i++)
	{
		backColors[i] = color1;
		//strip1.setPixelColor(i, color1.getR(), color1.getG(), color1.getB());
	}
}

void pattern_pulse()
{
	int r, g, b;
	double scale = sin(PI * 2 * millis() / delayAnimationSpeed);
	if (scale > 0)
	{
		r = color1.getR() * scale;
		g = color1.getG() * scale;
		b = color1.getB() * scale;
	}
	else
	{
		r = color2.getR() * scale * -1;
		g = color2.getG() * scale * -1;
		b = color2.getB() * scale * -1;
	}

	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		//strip1.setPixelColor(i, r, g, b);
		frontColors[i] = RGBColor(r, g, b);
	}

	for (int i = 0; i < LED_COUNT_BACK; i++)
	{
		backColors[i] = RGBColor(r, g, b);
	}
}

int len = 5;
void pattern_scroll()
{
	int r, g, b;
	int offset = millis() / delayAnimationSpeed;
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		if ((i + offset) / len % 2 == 1)
		{
			r = color1.getR();
			g = color1.getG();
			b = color1.getB();
		}
		else
		{
			r = color2.getR();
			g = color2.getG();
			b = color2.getB();
		}
		//strip1.setPixelColor(i, r, g, b);
		frontColors[i] = RGBColor(r, g, b);
	}
}

int bounds(int value)
{
	return min(255, max(0, value));
}

void pattern_scrollsmooth()
{
	// where pattern_scroll used modulus, scrollsmooth uses sin waves
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		double scaleA = abs(sin(PI * 2 + PI / 2 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed));
		double scaleB = abs(sin(PI * 2 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed));
		int r = bounds(color1.getR() * scaleA + color2.getR() * scaleB);
		int g = bounds(color1.getG() * scaleA + color2.getG() * scaleB);
		int b = bounds(color1.getB() * scaleA + color2.getB() * scaleB);

		//strip1.setPixelColor(i, r, g, b);
		frontColors[i] = RGBColor(r, g, b);
	}

	for (int i = 0; i < LED_COUNT_BACK; i++)
	{
		double scaleA = abs(sin(PI * 2 + PI / 2 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed));
		double scaleB = abs(sin(PI * 2 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed));
		int r = bounds(color1.getR() * scaleA + color2.getR() * scaleB);
		int g = bounds(color1.getG() * scaleA + color2.getG() * scaleB);
		int b = bounds(color1.getB() * scaleA + color2.getB() * scaleB);

		//strip1.setPixelColor(i, r, g, b);
		backColors[i] = RGBColor(r, g, b);
	}
}

int position = 0;
bool ascending = false;
bool firstColor = false;
void pattern_wipeLeft()
{
	if (ascending)
		position++;
	else
		position--;

	if (position >= LED_COUNT_FRONT)
	{
		position = 0;
		firstColor = !firstColor;
	}
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		if (i == position)
		{
			if (firstColor)
				frontColors[i] = color1;
			else
				frontColors[i] = color2;
		}
		else
		{
			double decay = 0.95;
			frontColors[i] = RGBColor(frontColors[i].getR() * decay, frontColors[i].getG() * decay, frontColors[i].getB() * decay);
		}
	}
}

void pattern_wipeRight()
{

}

void pattern_larson()
{
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		//frontColors[i] = color2;
		frontColors[i] = color2;
	}
	int width = 2;
	int onPin = LED_COUNT_FRONT / 2 + ((LED_COUNT_FRONT / 2) - width) * sin(PI * 2 + millis() / delayAnimationSpeed);
	Serial.println(onPin);
	for (int i = -width; i <= width; i++)
	{
		double amplitude = sin(PI / (double)2 * (i / (double)width));
		//Serial.println(i);
		if (onPin + i > LED_COUNT_FRONT) {
			Serial.print("ERR: ");
			Serial.println(onPin + i);
		}
		int pin = max(LED_COUNT_FRONT, min(0, i + onPin));
		//frontColors[pin] = RGBColor(color1.getR() * amplitude, color1.getG() * amplitude, color1.getB() *amplitude);
	}
}

void pattern_randBlink()
{

}

void pattern_decay()
{

}

// actions
void resetAction()
{
	action = ' ';
	//actionTimer = millis();
}

void action_setColor()
{

}

void action_fadeColor()
{

}

void action_flash()
{

}

void action_wipeLeft()
{

}

void action_wipeRight()
{

}

void action_wipeCenter()
{

}

void preset_redWhiteBlue()
{
	int r, g, b;
	for (int i = 0; i < LED_COUNT_FRONT; i++)
	{
		double scale3 = abs(pow(sin(PI * 2 + PI / 3 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed), 3));
		double scale2 = abs(pow(sin(PI * 2 + PI * 2 / 3 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed), 3));
		double scale1 = abs(pow(sin(PI * 2 + PI / LED_COUNT_FRONT * i + millis() / delayAnimationSpeed), 3));
		r = bounds(255 * scale1 + 255 * scale2);
		g = bounds(255 * scale2);
		b = bounds(255 * scale2 + 255 * scale3);
		frontColors[i] = RGBColor(r, g, b);
	}
}