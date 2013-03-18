#ifndef __VECTOR3_HPP__
#define __VECTOR3_HPP__

#include <limits.h>
#include <math.h>

#ifndef __glew_h__
#include <GL/glew.h>
#endif


static const double PI = 3.14159265358979323846;

const double PIOVER180 = PI/180;

struct Vector3
{
	float x, y, z;

	Vector3();
	Vector3(float X, float Y, float Z);
	Vector3(float angle);
	virtual ~Vector3() {}
	
	Vector3 operator-() const;
	Vector3 operator-(Vector3 vector) const;
	Vector3 operator-(float scalar);
	
	Vector3 operator+(Vector3 vector);
	Vector3 operator+(float scalar);

	Vector3 operator*(Vector3 vector);
	Vector3 operator*(float scalar);

	Vector3 operator/(Vector3 vector);
	Vector3 operator/(float scalar);

	bool operator<(Vector3 vector);
	bool operator>(Vector3 vector);
	bool operator<=(Vector3 vector);
	bool operator>=(Vector3 vector);
	bool operator==(Vector3 vector);
	bool operator!=(Vector3 vector);

	Vector3 &Vector3::operator=(const Vector3 & vector);

	/*const float *v()
	{
		V[0] = x;
		V[1] = y;
		V[2] = z;

		return V;
	}*/

	static inline Vector3 InvalidVector()
	{
		return Vector3((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);
	}
};

Vector3		CreateNormal(Vector3 v1, Vector3 v2, Vector3 v3);
Vector3		CreateNormal(Vector3 *v);
Vector3		Cross(Vector3 v1, Vector3 v2);
float		InvSqrt(float x);
float		Magnitude(Vector3 vector);
float		Magnitude(float x, float y, float z);
//float		Magnitude(float x, float y);
Vector3		Normalize(Vector3 vector);
Vector3		Normalize(float x, float y, float z);
float		Dot(Vector3 vector1, Vector3 vector2);
float		AngleBetweenVectors(Vector3 vector1, Vector3 vector2);
float		AngleBetweenVectorsD(Vector3 vector1, Vector3 vector2);
float		PlaneDist(Vector3 vector1, Vector3 vector2);
Vector3		Lerp(Vector3 v1, Vector3 v2, float k);
Vector3		Reflect(Vector3 n, Vector3 v);
#endif