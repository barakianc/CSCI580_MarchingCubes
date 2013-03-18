#include "vector2.hpp"

Vector2::Vector2()
{
	x = y = 0;
}

Vector2::Vector2(float X, float Y)
{
	x = X;
	y = Y;
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator-(Vector2 vector) const
{ 
	return Vector2( x - vector.x, y - vector.y);
}

Vector2 Vector2::operator+(Vector2 vector)
{ 
	return Vector2( x + vector.x, y + vector.y);
}

Vector2 Vector2::operator+(float scalar)
{
	Vector2 temp(x, y);
	float mag = sqrt(x*x + y*y);
	if(mag == 0) mag=0.0001f;

	temp.x /= mag;
	temp.y /= mag;

	temp.x *= scalar;
	temp.y *= scalar;

	return temp;
}

Vector2 Vector2::operator*(Vector2 vector)
{ 
	return Vector2( x * vector.x, y * vector.y);
}

Vector2 Vector2::operator*(float scalar)
{ 
	return Vector2( x*scalar, y*scalar);
}

Vector2 Vector2::operator/(Vector2 vector)
{ 
	return Vector2( x/vector.x, y/vector.y);
}

Vector2 Vector2::operator/(float scalar)
{ 
	return Vector2( x/scalar, y/scalar);
}

bool Vector2::operator<(Vector2 vector)
{
	if((x < vector.x) && (y < vector.y))
		return true;
	else
		return false;
}

bool Vector2::operator>(Vector2 vector)
{
	if((x > vector.x) && (y > vector.y))
		return true;
	else
		return false;
}

bool Vector2::operator<=(Vector2 vector)
{
	if((x <= vector.x) && (y <= vector.y))
		return true;
	else
		return false;
}

bool Vector2::operator>=(Vector2 vector)
{
	if((x >= vector.x) && (y >= vector.y))
		return true;
	else
		return false;
}

bool Vector2::operator==(Vector2 vector)
{
	if((vector.x == x) && (vector.y == y))
		return true;
	else
		return false;
}

bool Vector2::operator!=(Vector2 vector)
{
	if((vector.x == x) && (vector.y == y))
		return false;
	else
		return true;
}

Vector2 &Vector2::operator=(const Vector2 & vector)
{
	x = vector.x;
	y = vector.y;
	return *this;
}