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
	char r;
	char g;
	char b;
};

