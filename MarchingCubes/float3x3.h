#ifndef __FLOAT3x3_H__
#define __FLOAT3x3_H__

#include <iostream>
#include <math.h>

#ifndef __VECTOR2_HPP__
#include "vector2.hpp"
#endif
#ifndef __VECTOR3_HPP__
#include "vector3.hpp"
#endif

using namespace std;

class float3x3
{
private:

public:
	double m[9];

	float3x3();
	float3x3(double t[]);
	float3x3(double m0, double m1, double m2,
			double m3, double m4, double m5,
			double m6, double m7, double m8);
	virtual ~float3x3();

	void		FillMat(float3x3 matrix);
	void		FillMat(double m0, double m1, double m2,
						double m3, double m4, double m5,
						double m6, double m7, double m8);
	void		Print();

	bool		operator==(float3x3 mat2);
	void		operator=(float3x3 mat2);
	float3x3	operator+(float3x3 mat2);
	float3x3	operator-(float3x3 mat2);
	float3x3	operator*(float3x3 mat2);
	float3x3	operator*(double scalar);
	Vector3		operator*(Vector3 v);
	Vector2		operator*(Vector2 v);
	
	float3x3	Adjoint();
	float3x3	Transpose();
	float3x3	Inverse();
	double		Determinant();
	float3x3	Identity();
	float3x3	Scale(double x, double y);
	float3x3	Scale(Vector2 v);
	float3x3	SetTranslation(double x, double y);
	float3x3	SetTranslation(Vector2 v);
	float3x3	SetTranslation(Vector3 v);
	float3x3	SetRotation(double angle);
	Vector2		GetTranslation();
	//Vector2		GetRotation();

private:
	double Determinant2x2(double m[]);
};


#endif