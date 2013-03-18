#ifndef __COLOR_HPP__
#define __COLOR_HPP__

struct Color
{
	float r, g, b, a;

	Color();
	Color(Color &c);
	Color(float r, float g, float b, float a);

	virtual ~Color() {}

	Color &Color::operator=(const Color & color);
};

Color::Color()	
{
	Color(1.0, 1.0, 1.0, 1.0);
}

Color::Color(Color &c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color &Color::operator=(const Color & color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;
	return *this;
}

#endif