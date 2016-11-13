#include <Adafruit_NeoPixel.h>
#include "Patterns.h"
#include "RGBColor.h"
#include <stdlib.h>

#define NUMBER_OF_STRIPS 2 

// Configuration
#define BRIGHTNESS 30
#define BAUD_RATE 115200

struct StripData {
	int startIndex; // the starting index of the strips color data
	int length; // the length of the strip
	bool mirrored; // is the strip mirrored? normally pin # increments from left to right
	int pin; // the pin that the strip is connected to
};

StripData strips[NUMBER_OF_STRIPS] = {
	/*{ 0, 128, true, 7 },
	{ 128, 119, false, 6 }*/

	{0, 15, true, D7 },
	{ 15, 15, false, D6 }
};
//#define LED_COUNT_FRONT 247
#define LED_COUNT 30

Adafruit_NeoPixel NeoPixels[NUMBER_OF_STRIPS];
//= Adafruit_NeoPixel(STRIP1_LENGTH, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

RGBColor colors[LED_COUNT];

char pattern;
char action;
RGBColor color1;
RGBColor color2;
double delayAnimationSpeed;
double delayHold;
bool actionRunning;
int width;

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
	pattern = PATTERN_SCROLLSMOOTH;
	color1 = RGBColor(255,0,0); //255 185 0
	color2 = RGBColor(0, 0, 255); // 255 00 255
	delayAnimationSpeed = 250;
	delayHold = 0;
	width = 30;
}

void getSerialData()
{
	// if serial contains data
	if (Serial.available() > 0)
	{
		// set the buffer to the next line of input
		serialBuffer = Serial.readStringUntil('\n');
		Serial.flush();

		int indexStart = serialBuffer.indexOf(START_OF_MESSAGE);
		int indexEnd = serialBuffer.indexOf(END_OF_MESSAGE);
		int stripNumToChange = 0;

		if (indexStart != -1 &&
			indexEnd != -1 &&
			indexStart < indexEnd)
		{
			// message contains all of the data
			String message = serialBuffer.substring(indexStart + String(START_OF_MESSAGE).length(), indexEnd);
			// serialBuffer starts at the end of the old buffer
			//serialBuffer = serialBuffer.substring(indexEnd, serialBuffer.length() - 1);
			//Serial.flush();
			//Serial.println(message);
			// parse data
			//Serial.print("Pattern");
			pattern = message.charAt(0);
			action = pattern;
			//Serial.println(pattern);
			int r = message.substring(1, 4).toInt();
			int g = message.substring(4, 7).toInt();
			int b = message.substring(7, 10).toInt();
			int r2 = message.substring(10, 13).toInt();
			int g2 = message.substring(13, 16).toInt();
			int b2 = message.substring(16, 19).toInt();
			/*Serial.println(r);
			Serial.println(g);
			Serial.println(b);
			Serial.println(r2);
			Serial.println(g2);
			Serial.println(b2);*/
			color1 = RGBColor(r, g, b);
			color2 = RGBColor(r2, g2, b2);
			delayAnimationSpeed = message.substring(19, 23).toInt();
			delayHold = message.substring(23, 27).toInt();
			width = message.substring(27, 30).toInt();
		}
	}
}

void loop()
{
	//delay(5);
	// do serial stuff
	getSerialData();
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
	for (int stripNumber = 0; stripNumber < NUMBER_OF_STRIPS; stripNumber++)
	{
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
			if (!strips[stripNumber].mirrored)
			{
				int index = c + strips[stripNumber].startIndex;
				NeoPixels[stripNumber].setPixelColor(c, colors[index].getR(), colors[index].getG(), colors[index].getB());
			}
			else
			{
				int index = strips[stripNumber].length - c + strips[stripNumber].startIndex;
				NeoPixels[stripNumber].setPixelColor(c, colors[index].getR(), colors[index].getG(), colors[index].getB());
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
	for (int i = 0; i < LED_COUNT; i++)
	{
		if (blinkToggle)
		{
			colors[i] = color1;
		}
		else
		{
			colors[i] = color2;
		}
	}
}

void pattern_solidColor()
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		colors[i] = color1;
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

	for (int i = 0; i < LED_COUNT; i++)
	{
		//strip1.setPixelColor(i, r, g, b);
		colors[i] = RGBColor(r, g, b);
	}
}

void pattern_scroll()
{
	int r, g, b;
	int offset = millis() / delayAnimationSpeed;
	for (int i = 0; i < LED_COUNT; i++)
	{
		if ((i + offset) / width % 2 == 1)
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
		colors[i] = RGBColor(r, g, b);
	}
}

int bounds(int value)
{
	return min(255, max(0, value));
}

void pattern_scrollsmooth()
{
	// where pattern_scroll used modulus, scrollsmooth uses sin waves
	for (int i = 0; i < LED_COUNT; i++)
	{
		double scaleA = abs(sin(PI / 2 + 2*PI * i / (double)width + millis() / delayAnimationSpeed));
		double scaleB = abs(sin(2* PI * i / (double)width + millis() / delayAnimationSpeed));
		int r = bounds(color1.getR() * scaleA + color2.getR() * scaleB);
		int g = bounds(color1.getG() * scaleA + color2.getG() * scaleB);
		int b = bounds(color1.getB() * scaleA + color2.getB() * scaleB);

		//strip1.setPixelColor(i, r, g, b);
		colors[i] = RGBColor(r, g, b);
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

	if (position >= LED_COUNT)
	{
		position = 0;
		firstColor = !firstColor;
	}
	for (int i = 0; i < LED_COUNT; i++)
	{
		if (i == position)
		{
			if (firstColor)
				colors[i] = color1;
			else
				colors[i] = color2;
		}
		else
		{
			double decay = 0.95;
			colors[i] = RGBColor(colors[i].getR() * decay, colors[i].getG() * decay, colors[i].getB() * decay);
		}
	}
}

void pattern_wipeRight()
{

}

void pattern_larson()
{
	// put all lights to color2
	for (int i = 0; i < LED_COUNT; i++)
	{
		//frontColors[i] = color2;
		colors[i] = color2;
	}
	
	int onPin =(int)(LED_COUNT / 2 + ((LED_COUNT / 2) - width) * sin(PI * 2 * millis() / delayAnimationSpeed));
	//Serial.println(onPin);
	//int onPin = (int)(LED_COUNT / 2 + ((LED_COUNT / 2)) * sin(PI * 2 * millis() / delayAnimationSpeed));
	//colors[onPin] = color1;
	//Serial.println(onPin);
	for (int i = -width; i <= width; i++)
	{
		colors[onPin + i] = color1;
		//double amplitude = sin(PI / (double)2 * (i / (double)width));
		////Serial.println(i);
		//if (onPin + i > LED_COUNT_FRONT) {
		//	//Serial.print("ERR: ");
		//	//Serial.println(onPin + i);
		//}
		//int pin = max(LED_COUNT_FRONT, min(0, i + onPin));
		//int r = bounds(amplitude * color1.getR());
		//int g = bounds(amplitude * color1.getG());
		//int b = bounds(amplitude * color1.getB());
		//colors[pin] = RGBColor(r,g,b);
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
	for (int i = 0; i < LED_COUNT; i++)
	{
		double scale3 = abs(pow(sin(PI * 2 + PI / 3 + PI / (double)width * i + millis() / delayAnimationSpeed), 3));
		double scale2 = abs(pow(sin(PI * 2 + PI * 2 / 3 + PI / (double)width * i + millis() / delayAnimationSpeed), 3));
		double scale1 = abs(pow(sin(PI * 2 + PI / (double)width * i + millis() / delayAnimationSpeed), 3));
		r = bounds(255 * scale1 + 255 * scale2);
		g = bounds(255 * scale2);
		b = bounds(255 * scale2 + 255 * scale3);
		colors[i] = RGBColor(r, g, b);
	}
}