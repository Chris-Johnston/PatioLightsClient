#include "RGBColor.h"

RGBColor::RGBColor()
{
	r = VALUE_MIN;
	g = VALUE_MIN;
	b = VALUE_MIN;
}

RGBColor::RGBColor(int red, int green, int blue)
{
	r = (char)clampBounds(red);
	g = (char)clampBounds(green);
	b = (char)clampBounds(blue);
}

int RGBColor::getR()
{
	return (int)r;
}

int RGBColor::getG()
{
	return (int)g;
}

int RGBColor::getB()
{
	return (int)b;
}

RGBColor::~RGBColor()
{
}

int RGBColor::clampBounds(int value)
{
	return (value > VALUE_MAX) ? VALUE_MAX : (value < VALUE_MIN) ? VALUE_MIN : value;
}
