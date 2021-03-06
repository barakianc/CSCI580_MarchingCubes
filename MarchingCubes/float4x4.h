#ifndef __FLOAT4X4_H__
#define __FLOAT4X4_H__

#include <iostream>
#include <math.h>
#include "float3x3.h"
#include "vector3.hpp"

using namespace std;

class float4x4
{
public:
	double m[16];

	float4x4();
	float4x4(double t[]);
	float4x4(float3x3 mat);
	float4x4(double m0, double m1, double m2, double m3,
			double m4, double m5, double m6, double m7,
			double m8, double m9, double m10, double m11,
			double m12, double m13, double m14, double m15);
	virtual ~float4x4();

	void		fillMat(float4x4 matrix);
	void		fillMat(double m0, double m1, double m2, double m3,
						double m4, double m5, double m6, double m7,
						double m8, double m9, double m10, double m11,
						double m12, double m13, double m14, double m15);
	void		print();

	void		operator=(float4x4 mat2);
	float4x4	operator+(float4x4 mat2);
	float4x4	operator-(float4x4 mat2);
	float4x4	operator*(float4x4 mat2);
	float4x4	operator*(double scalar);
	Vector3		operator*(Vector3 v);
	
	float4x4	adjoint();
	float4x4	transpose();
	float4x4	inverse();
	double		determinant();
	void		makeIdentity();
	float4x4	translate(Vector3 trans);
	float4x4	rotate(double angle, double x, double y, double z);

	float4x4	CreateProjectionMatrix(float fovy, float aspectRatio, float nearPlane, float farPlane);

private:
	double determinant2x2(double m[]);
	double determinant3x3(double m[]);
};

static const float4x4 IDENTITY_MATRIX = float4x4(1,0,0,0,
												 0,1,0,0,
												 0,0,1,0,
												 0,0,0,1);


#endif