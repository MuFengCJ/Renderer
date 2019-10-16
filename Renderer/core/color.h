#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color(float r, float g, float b, float a);
	//~Color();

	float r, g, b, a;

	static Color White;
	static Color Black;
	static Color Red;
	static Color Cyan;

private:

};

#endif
