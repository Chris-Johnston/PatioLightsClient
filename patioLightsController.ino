#include <Adafruit_NeoPixel.h>
#include "Patterns.h"
#include "RGBColor.h"
#include <stdlib.h>

//#include <pgmStrToRAM.h>
//#include <MemoryFree.h>

//#include "Arduino.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define NUMBER_OF_STRIPS 2 

// Configuration
#define BRIGHTNESS 10
#define BAUD_RATE 250000

struct StripData {
	int startIndex; // the starting index of the strips color data
	int length; // the length of the strip
	bool mirrored; // is the strip mirrored? normally pin # increments from left to right
	int pin; // the pin that the strip is connected to
};

StripData strips[NUMBER_OF_STRIPS] = {
	{ 0, 128, true, 7 },
	{ 128, 119, false, 6 }

	/*{0, 15, true, D7 },
	{ 15, 15, false, D6 }*/
};

#define LED_COUNT 247
#define ARR_LEN 247*3 // led count * 3

Adafruit_NeoPixel NeoPixels[NUMBER_OF_STRIPS];
//= Adafruit_NeoPixel(STRIP1_LENGTH, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

//RGBColor colors[LED_COUNT];
uint8_t colors[ARR_LEN]; // r g b for each val

void setColor(int i, int r, int g, int b)
{
	int index = min(i, ARR_LEN) * 3;
	colors[index] = bounds(r);
	colors[index + 1] = bounds(g);
	colors[index + 2] = bounds(b);
}

void setColor(int i, RGBColor color)
{
	int index = min(i, ARR_LEN) * 3;
	colors[index] = bounds(color.getR());
	colors[index+1] = bounds(color.getG());
	colors[index+2] = bounds(color.getB());
}

char pattern;
char action;
RGBColor color1;
RGBColor color2;
double delayAnimationSpeed;
double delayHold;
bool actionRunning;
int width;

#define SERIALMESSAGELENGTH 37
char serialBuffer[37];

void setup()
{
	Serial.begin(BAUD_RATE);
	//Serial.println("Start");
	Serial.setTimeout(100);

	// initialize the strips
	for (int i = 0; i < NUMBER_OF_STRIPS; i++)
	{
		NeoPixels[i] = Adafruit_NeoPixel(strips[i].length, strips[i].pin, NEO_GRB + NEO_KHZ800);
		NeoPixels[i].setBrightness(BRIGHTNESS);
		NeoPixels[i].begin();
	}

	//pattern = PATTERN_SCROLL;
	pattern = PATTERN_SCROLLSMOOTH;
	color1 = RGBColor(0,255,0); 
	color2 = RGBColor(0, 0, 255);
	delayAnimationSpeed = 500;
	delayHold = 0;
	width = 7;
}

// so it turns out that disabling interrupts by the neopixel library causes issues for serial.read
// https://learn.adafruit.com/neopixels-and-servos/overview#the-root-of-the-problem

// issue with serialEvent
// data wasn't being sent over correctly when neopixel show was being used
//void serialEvent()
//{
//	getSerialData();
//}

void getSerialData()
{
	// I could only draw when I send a draw command but that's slow
	// also if I did anything where the arduino cannot work alone, that means that it needs the script running full time, which is no good

	//Serial.println("Ser Data");
	//noInterrupts();
	//memset(serialBuffer, 0, 30);
	// if serial contains data
	if (Serial.available() > 0)
	{
		String s;
		// when serial data arrives, I'm going to flood it with useless data just so I know that serial is coming in
		// then remove that data and send the legit data
		if (Serial.read() == 'x')
		{
			//Serial.flush();
			/*Serial.println("Useless data found. Waiting for more.");
			Serial.println("Useless data found. Waiting for more.");
			Serial.println("Useless data found. Waiting for more.");
			Serial.println("Useless data found. Waiting for more.");*/

			Serial.flush();

			while (!(Serial.available() > 0))
			//while (Serial.available() < 1)
			{
				//Serial.println(Serial.peek());
			}
			//Serial.println(Serial.peek());
			//s = Serial.readString();
			s = Serial.readString();
			
			// set the buffer to the next line of input

			//String s = Serial.readString();
			//Serial.println(s);
			int iLeft, iRight;
			iLeft = s.indexOf('s');
			iRight = s.indexOf('e');
			s = s.substring(iLeft, iRight);
			s.toCharArray(serialBuffer, SERIALMESSAGELENGTH);

			//Serial.println(s);
			//Serial.println(serialBuffer);
			Serial.flush();

			/*int indexStart = serialBuffer.indexOf(START_OF_MESSAGE);
			int indexEnd = serialBuffer.indexOf(END_OF_MESSAGE);
			int stripNumToChange = 0;

			if (indexStart != -1 &&
				indexEnd != -1 &&
				indexStart < indexEnd)*/
			if (serialBuffer[0] == START_OF_MESSAGE)
			{
				//Serial.println("lkjsdflkjd");
				// message contains all of the data
				//serialBuffer = serialBuffer.substring(indexStart + String(START_OF_MESSAGE).length(), indexEnd);
				for (int i = 0; i < SERIALMESSAGELENGTH; i++)
				{
					serialBuffer[i] = serialBuffer[i + 1];
				}

				Serial.println(serialBuffer);

				//pattern = serialBuffer.charAt(0);
				char tempPattern = serialBuffer[0];
				//Serial.println(pattern);
				/*action = pattern;*/
				//Serial.println(pattern);

				/*int r = serialBuffer.substring(1, 4).toInt();
				int g = serialBuffer.substring(4, 7).toInt();
				int b = serialBuffer.substring(7, 10).toInt();
				int r2 = serialBuffer.substring(10, 13).toInt();
				int g2 = serialBuffer.substring(13, 16).toInt();
				int b2 = serialBuffer.substring(16, 19).toInt();*/
				int r = ((String)"" + serialBuffer[1] + serialBuffer[2] + serialBuffer[3]).toInt();
				int g = ((String)"" + serialBuffer[4] + serialBuffer[5] + serialBuffer[6]).toInt();
				int b = ((String)"" + serialBuffer[7] + serialBuffer[8] + serialBuffer[9]).toInt();

				int r2 = ((String)"" + serialBuffer[10] + serialBuffer[11] + serialBuffer[12]).toInt();
				int g2 = ((String)"" + serialBuffer[13] + serialBuffer[14] + serialBuffer[15]).toInt();
				int b2 = ((String)"" + serialBuffer[16] + serialBuffer[17] + serialBuffer[18]).toInt();


				/*RGBColor tempColor1 = RGBColor(r, g, b);
				RGBColor tempColor2 = RGBColor(r2, g2, b2);
*/
				//Serial.println("c1" + r + g + b);
				//Serial.println("c2" + r2 + g2 + b2);

				//delayAnimationSpeed = serialBuffer.substring(19, 23).toInt();
				int tempDelayAnimationSpeed = ((String)"" + serialBuffer[19] + serialBuffer[20] + serialBuffer[21] + serialBuffer[22]).toInt();
				//Serial.println(delayAnimationSpeed);
				//delayHold = serialBuffer.substring(23, 27).toInt();
				int tempDelayHold = ((String)"" + serialBuffer[23] + serialBuffer[24] + serialBuffer[25] + serialBuffer[26]).toInt();
				//Serial.println(delayHold);
				//width = serialBuffer.substring(27, 30).toInt();
				int tempWidth = ((String)"" + serialBuffer[27] + serialBuffer[28] + serialBuffer[29]).toInt();
				//Serial.println(width);

				// checksum
				int val = (int)(r + g + b + r2 + g2 + b2 + tempDelayAnimationSpeed + tempDelayHold + tempWidth) % 99;
				int chk = ((String)"" + serialBuffer[30] + serialBuffer[31]).toInt();
				// 99 override value so I can still type things in by hand
				if (chk == val || chk==99)
				{
					// checksum matches and set the values
					pattern = tempPattern;
					color1 = RGBColor(r, g, b);
					color2 = RGBColor(r2, g2, b2);
					delayAnimationSpeed = tempDelayAnimationSpeed;
					delayHold = tempDelayHold;
					width = tempWidth;
				}
			}
		}
	}
	interrupts();
}

void loop()
{
	//noInterrupts();
	// debug memory
	/*Serial.print(millis());
	Serial.print(" Ram: ");
	Serial.println(freeMemory());*/

	//delay(20);
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

	//noInterrupts();
	// update the strips
	for (int stripNumber = 0; stripNumber < NUMBER_OF_STRIPS; stripNumber++)
	{
		for (int c = 0; c < strips[stripNumber].length; c++)
		{
			if (!strips[stripNumber].mirrored)
			{
				// update normal strip
				int index = (c + strips[stripNumber].startIndex) * 3;
				//NeoPixels[stripNumber].setPixelColor(c, colors[index].getR(), colors[index].getG(), colors[index].getB());
				NeoPixels[stripNumber].setPixelColor(c, (int)colors[index], (int)colors[index + 1], (int)colors[index + 2]);
			}
			else
			{
				// update mirrored strip
				int index = (strips[stripNumber].length - c + strips[stripNumber].startIndex) * 3;
				//NeoPixels[stripNumber].setPixelColor(c, colors[index].getR(), colors[index].getG(), colors[index].getB());
				NeoPixels[stripNumber].setPixelColor(c, (int)colors[index], (int)colors[index + 1], (int)colors[index + 2]);
			}
		}
		noInterrupts();
		NeoPixels[stripNumber].show();
		interrupts();
	}
	interrupts();
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
			//colors[i] = color1;
			setColor(i, color1);

		}
		else
		{
			//colors[i] = color2;
			setColor(i, color2);
		}
	}
}

void pattern_solidColor()
{
	for (int i = 0; i < LED_COUNT; i++)
	{
		//colors[i] = color1;
		setColor(i, color1);

		//strip1.setPixelColor(i, color1.getR(), color1.getG(), color1.getB());
	}
}

void pattern_pulse()
{
	int r, g, b;
	//double scale = sin(PI * 2 * millis() / delayAnimationSpeed);
	double scale = triangleAbs(PI * 2 * millis() / delayAnimationSpeed);
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
		//colors[i] = RGBColor(r, g, b);
		setColor(i, r, g, b);
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
		//colors[i] = RGBColor(r, g, b);
		setColor(i, r, g, b);
	}
}

uint8_t bounds(int value)
{
	return min(255, max(0, value));
}

void pattern_scrollsmooth()
{
	// where pattern_scroll used modulus, scrollsmooth uses sin waves
	for (int i = 0; i < LED_COUNT; i++)
	{
		/*double scaleA = abs(sin(PI / 2 + 2*PI * i / (double)width + millis() / delayAnimationSpeed));
		double scaleB = abs(sin(2* PI * i / (double)width + millis() / delayAnimationSpeed));*/
		/*double scaleA = abs(triangle(PI / 2 + 2 * PI * i / (double)width + millis() / delayAnimationSpeed));
		double scaleB = abs(triangle(2 * PI * i / (double)width + millis() / delayAnimationSpeed));*/

		double scaleA = triangleAbs(1 + i / (double)width + millis() / delayAnimationSpeed);
		double scaleB = triangleAbs(i / (double)width + millis() / delayAnimationSpeed);
		uint8_t r = (uint8_t)bounds(color1.getR() * scaleA + color2.getR() * scaleB);
		uint8_t g = (uint8_t)bounds(color1.getG() * scaleA + color2.getG() * scaleB);
		uint8_t b = (uint8_t)bounds(color1.getB() * scaleA + color2.getB() * scaleB);

		//strip1.setPixelColor(i, r, g, b);
		//colors[i] = RGBColor(r, g, b);
		setColor(i, r, g, b);
	}
}

// returns from 1 to 0
double triangleAbs(double p)
{
	return 1.0 - fabs(fmod(p, 2.0) - 1.0);
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
			{
				setColor(i, color1);
			}
			else
			{
				setColor(i, color2);
			}
			//	colors[i] = color1;
			//else
			//	colors[i] = color2;
		}
		else
		{
			double decay = 0.95;
			//colors[i] = RGBColor(colors[i].getR() * decay, colors[i].getG() * decay, colors[i].getB() * decay);
			//todo setColor(i, RGBColor(colors[i].getR() * decay, colors[i].getG() * decay, colors[i].getB() * decay));
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
		//colors[i] = color2;
		setColor(i, color2);
	}
	
	int onPin =(int)(LED_COUNT / 2 + ((LED_COUNT / 2) - width) * sin(PI * 2 * millis() / delayAnimationSpeed));
	//Serial.println(onPin);
	//int onPin = (int)(LED_COUNT / 2 + ((LED_COUNT / 2)) * sin(PI * 2 * millis() / delayAnimationSpeed));
	//colors[onPin] = color1;
	//Serial.println(onPin);
	for (int i = -width; i <= width; i++)
	{
		//colors[onPin + i] = color1;
		setColor(onPin + i, color1);

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

void preset_redWhiteBlue()
{
	uint8_t r, g, b;
	for (int i = 0; i < LED_COUNT; i++)
	{
		//double scale3 = abs(pow(sin(PI * 2 + PI / 3 + PI / (double)width * i + millis() / delayAnimationSpeed), 3));
		//double scale2 = abs(pow(sin(PI * 2 + PI * 2 / 3 + PI / (double)width * i + millis() / delayAnimationSpeed), 3));
		//double scale1 = abs(pow(sin(PI * 2 + PI / (double)width * i + millis() / delayAnimationSpeed), 3));
		double p = i / (double)width;
		double scale3 = pow(triangleAbs(p + millis() / delayAnimationSpeed), 2);
		double scale2 = pow(triangleAbs(0.66 + p + millis() / delayAnimationSpeed), 2);
		double scale1 = pow(triangleAbs(1.33 + p + millis() / delayAnimationSpeed), 2);
		r = (uint8_t)bounds(255 * scale1 + 255 * scale2);
		g = (uint8_t)bounds(255 * scale2);
		b = (uint8_t)bounds(255 * scale2 + 255 * scale3);
		//colors[i] = RGBColor(r, g, b);
		setColor(i, r, g, b);
	}
}