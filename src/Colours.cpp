#include <Colours.h>

Colours::Colours(int r, int g, int b, int x, int y)
{
	myColour.r = r;
	myColour.g = g;
	myColour.b = b;
	myColour.a = 180;
	myShape.setX(x);
	myShape.setY(y);
	myShape.setHeight(30);
	myShape.setWidth(30);
}