#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <limits.h>
#include <math.h>

#ifndef __glew_h__
#include <GL/glew.h>
#endif

struct Vector2
{
public:
	float x, y;

	Vector2();
	Vector2(float X, float Y);
	virtual ~Vector2() {}

	Vector2 operator-();
	Vector2 operator-(Vector2 vector) const;
	
	Vector2 operator+(Vector2 vector);
	Vector2 operator+(float scalar);

	Vector2 operator*(Vector2 vector);
	Vector2 operator*(float scalar);

	Vector2 operator/(Vector2 vector);
	Vector2 operator/(float scalar);

	bool operator<(Vector2 vector);
	bool operator>(Vector2 vector);
	bool operator<=(Vector2 vector);
	bool operator>=(Vector2 vector);
	bool operator==(Vector2 vector);
	bool operator!=(Vector2 vector);

	Vector2 &Vector2::operator=(const Vector2 & vector);

	/*const GLdouble *v()
	{
		V[0] = x;
		V[1] = y;

		return V;
	}*/

	static inline Vector2 InvalidVector()
	{
		return Vector2((float)INT_MAX, (float)INT_MAX);
	}

	inline Vector2 Perp()
	{
		return Vector2(y,-x);
	}
};

inline float	Magnitude(Vector2 vector);
inline float	Magnitude(float x, float y);
inline Vector2	Normalize(Vector2 vector);
inline Vector2	Normalize(float x, float y);
inline float Dot(Vector2 v1, Vector2 v2);

inline float	Magnitude(Vector2 vector)
{
	return sqrt((vector.x*vector.x) + (vector.y*vector.y));
}

inline float	Magnitude(float x, float y)
{
	return sqrt((x*x) + (y*y));
}

inline Vector2	Normalize(Vector2 vector)
{
	float mag = Magnitude(vector);

	if(mag <= 0.00001f)
	{
		mag = 0.00001f;
	}

	return Vector2(vector.x / mag, vector.y / mag);
}

inline Vector2	Normalize(float x, float y)
{
	float mag = Magnitude(x, y);

	if(mag <= 0.00001f)
	{
		mag = 0.00001f;
	}

	return Vector2(x / mag, y / mag);
}

inline float Dot(Vector2 v1, Vector2 v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

#endif