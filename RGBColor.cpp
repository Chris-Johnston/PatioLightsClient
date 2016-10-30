#include "RGBColor.h"

RGBColor::RGBColor()
{
	r = VALUE_MIN;
	g = VALUE_MIN;
	b = VALUE_MIN;
}

RGBColor::RGBColor(int red, int green, int blue)
{
	r = clampBounds(red);
	g = clampBounds(green);
	b = clampBounds(blue);
}

int RGBColor::getR()
{
	return r;
}

int RGBColor::getG()
{
	return g;
}

int RGBColor::getB()
{
	return b;
}

RGBColor::~RGBColor()
{
}

int RGBColor::clampBounds(int value)
{
	return (value > VALUE_MAX) ? VALUE_MAX : (value < VALUE_MIN) ? VALUE_MIN : value;
}
