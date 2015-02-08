#include "Color.h"

Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;
}

Color::Color(float red, float green, float blue)
{
	r = red;
	g = green;
	b = blue;
	a = 1.0f;
}
	
Color::Color(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}