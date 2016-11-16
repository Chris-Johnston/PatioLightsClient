#include "Arduino.h"

#pragma once
#define VALUE_MIN 0
#define VALUE_MAX 255
class RGBColor
{
public:
	RGBColor();
	RGBColor(int, int, int);
	~RGBColor();
	int getR();
	int getG();
	int getB();
private:
	int clampBounds(int);
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

