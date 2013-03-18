#include "float3x3.h"

float3x3::float3x3()
{
	memset((void*)m, 0, sizeof(double) * 9);
	m[0] = m[4] = m[8] = 1.0f;
}

float3x3::float3x3(double t[])
{
	m[0] = t[0];
	m[1] = t[1];
	m[2] = t[2];
	m[3] = t[3];
	m[4] = t[4];
	m[5] = t[5];
	m[6] = t[6];
	m[7] = t[7];
	m[8] = t[8];
}

float3x3::float3x3(double m0, double m1, double m2,
		double m3, double m4, double m5,
		double m6, double m7, double m8)
{
	m[0] = m0;
	m[1] = m1;
	m[2] = m2;
	m[3] = m3;
	m[4] = m4;
	m[5] = m5;
	m[6] = m6;
	m[7] = m7;
	m[8] = m8;
}

float3x3::~float3x3()
{
}

void		float3x3::FillMat(float3x3 matrix)
{
	m[0] = matrix.m[0];
	m[1] = matrix.m[1];
	m[2] = matrix.m[2];
	m[3] = matrix.m[3];
	m[4] = matrix.m[4];
	m[5] = matrix.m[5];
	m[6] = matrix.m[6];
	m[7] = matrix.m[7];
	m[8] = matrix.m[8];
}

void		float3x3::FillMat(double m0, double m1, double m2,
					double m3, double m4, double m5,
					double m6, double m7, double m8)
{
	m[0] = m0;
	m[1] = m1;
	m[2] = m2;
	m[3] = m3;
	m[4] = m4;
	m[5] = m5;
	m[6] = m6;
	m[7] = m7;
	m[8] = m8;
}

void		float3x3::Print()
{
	cout << "[0 3 6] ";
	cout << m[0] << " " << m[3] << " " << m[6] << endl;
	cout << "[1 4 7] ";
	cout << m[1] << " " << m[4] << " " << m[7] << endl;
	cout << "[2 5 8] ";
	cout << m[2] << " " << m[5] << " " << m[8] << endl;
}

bool		float3x3::operator==(float3x3 mat2)
{
	for(size_t i=0; i < 16; i++)
	{
		if(m[i] != mat2.m[i])
			return false;
	}
	return true;
}

void		float3x3::operator=(float3x3 mat2)
{	
	memcpy(m,mat2.m,sizeof(float)*9);
}

float3x3	float3x3::operator+(float3x3 mat2)
{
	float3x3 ret;

	ret.m[0] += mat2.m[0];
	ret.m[1] += mat2.m[1];
	ret.m[2] += mat2.m[2];
	ret.m[3] += mat2.m[3];
	ret.m[4] += mat2.m[4];
	ret.m[5] += mat2.m[5];
	ret.m[6] += mat2.m[6];
	ret.m[7] += mat2.m[7];
	ret.m[8] += mat2.m[8];

	return ret;
}

float3x3	float3x3::operator-(float3x3 mat2)
{
	float3x3 ret;

	for(int i=0; i<9; i++)
	{
		ret.m[i] = m[i] - mat2.m[i];
	}

	return ret;
}

float3x3	float3x3::operator*(float3x3 mat2)
{
	float3x3 t;

	t.m[0] = m[0]*mat2.m[0] + m[3]*mat2.m[1] + m[6]*mat2.m[2];
	t.m[1] = m[1]*mat2.m[0] + m[4]*mat2.m[1] + m[7]*mat2.m[2];
	t.m[2] = m[2]*mat2.m[0] + m[5]*mat2.m[1] + m[8]*mat2.m[2];
	
	t.m[3] = m[0]*mat2.m[3] + m[3]*mat2.m[4] + m[6]*mat2.m[5];
	t.m[4] = m[1]*mat2.m[3] + m[4]*mat2.m[4] + m[7]*mat2.m[5];
	t.m[5] = m[2]*mat2.m[3] + m[5]*mat2.m[4] + m[8]*mat2.m[5];
	
	t.m[6] = m[0]*mat2.m[6] + m[3]*mat2.m[7] + m[6]*mat2.m[8];
	t.m[7] = m[1]*mat2.m[6] + m[4]*mat2.m[7] + m[7]*mat2.m[8];
	t.m[8] = m[2]*mat2.m[6] + m[5]*mat2.m[7] + m[8]*mat2.m[8];

	return t;
}

float3x3	float3x3::operator*(double scalar)
{
	float3x3 ret = m;

	for(int i=0; i<9; i++)
		ret.m[i] = m[i] * scalar;

	return ret;
}

Vector3		float3x3::operator*(Vector3 v)
{
	Vector3 r;
	r.x = (float)(m[0]*v.x + m[3]*v.y + m[6]*v.z);
	r.y = (float)(m[1]*v.x + m[4]*v.y + m[7]*v.z);
	r.z = (float)(m[2]*v.x + m[5]*v.y + m[8]*v.z);

	return r;
}

Vector2		float3x3::operator*(Vector2 v)
{
	Vector2 r;

	r.x = (float)(m[0]*v.x + m[3]*v.y + m[6]);
	r.y = (float)(m[1]*v.x + m[4]*v.y + m[7]);

	return r;
}

float3x3	float3x3::Adjoint()
{
	float3x3 t;

	return t;
}

float3x3	float3x3::Transpose()
{
	float3x3 t;

	t.m[0] = m[0];
	t.m[1] = m[3];
	t.m[2] = m[6];

	t.m[3] = m[1];
	t.m[4] = m[4];
	t.m[5] = m[7];

	t.m[6] = m[2];
	t.m[7] = m[5];
	t.m[8] = m[8];

	return t;
}

float3x3	float3x3::Inverse()
{
	/*float3x3 ret = float3x3(m);

	double detInv = 1.0 / determinant();
	ret = ret * detInv;

	return ret;*/

	float3x3 rot = float3x3(m[0],m[3],0, m[1],m[4],0, 0,0,1);
	float3x3 tran= float3x3(1,0,0, 0,1,0, -1.0f * m[6],-1.0f * m[7],1);
	
	return rot * tran;

	/*// | a11 a12 a13 |-1             |   a33a22-a32a23  -(a33a12-a32a13)   a23a12-a22a13  |
	// | a21 a22 a23 |    =  1/DET * | -(a33a21-a31a23)   a33a11-a31a13  -(a23a11-a21a13) |
	// | a31 a32 a33 |               |   a32a21-a31a22  -(a32a11-a31a12)   a22a11-a21a12  |
	double detInv = 1.0 / determinant();
	float3x3 ret;

	ret.m[0] = m[8]*m[5] - m[5]*m[7];
	ret.m[1] = -(m[8]*m[1] - m[2]*m[7]);
	ret.m[2] = m[5]*m[1] - m[2]*m[4];

	ret.m[3] = -(m[8]*m[3] - m[5]*m[6]);
	ret.m[4] = m[8]*m[0] - m[2]*m[6];
	ret.m[5] = -(m[5]*m[0] - m[2]*m[3]);

	ret.m[6] = m[7]*m[3] - m[4]*m[6];
	ret.m[7] = -(m[7]*m[0] - m[1]*m[6]);
	ret.m[8] = m[4]*m[0] - m[1]*m[3];

	return ret * detInv;*/
}

inline double float3x3::Determinant2x2(double m[])
{
	return (m[0]*m[3]) - (m[1]*m[2]);
}

double		float3x3::Determinant()
{
	double det1 = 0;
	double det2 = 0;
	double det3 = 0;

	double mat2x2[4];

	mat2x2[0] = m[4]; mat2x2[2] = m[7];
	mat2x2[1] = m[5]; mat2x2[3] = m[8];
	det1 = (Determinant2x2(mat2x2) * m[0]);

	mat2x2[0] = m[1]; mat2x2[2] = m[7];
	mat2x2[1] = m[2]; mat2x2[3] = m[8];
	det2 = (Determinant2x2(mat2x2) * m[3]);

	mat2x2[0] = m[1]; mat2x2[2] = m[4];
	mat2x2[1] = m[2]; mat2x2[3] = m[5];
	det3 = (Determinant2x2(mat2x2) * m[6]);

	return det1 - det2 + det3;
}

float3x3	float3x3::Identity()
{
	return float3x3();
}


float3x3	float3x3::Scale(double x, double y)
{
	return float3x3(m);
}

float3x3	float3x3::Scale(Vector2 v)
{
	return float3x3(m);
}

float3x3	float3x3::SetTranslation(double x, double y)
{
	/*float3x3 ret = float3x3(m);

	ret.m[6] = x;
	ret.m[7] = y;*/

	m[6] = x;
	m[7] = y;

	return *this;
}

float3x3	float3x3::SetTranslation(Vector2 v)
{
	/*float3x3 ret = float3x3(m);

	ret.m[6] = v.x;
	ret.m[7] = v.y;*/

	m[6] = v.x;
	m[7] = v.y;

	return *this;
}

float3x3	float3x3::SetTranslation(Vector3 v)
{
	/*float3x3 ret;
	ret.FillMat(m);

	ret.m[6] = v.x;
	ret.m[7] = v.y;*/

	m[6] = v.x;
	m[7] = v.y;

	return *this;
}

float3x3	float3x3::SetRotation(double angle)
{
	float3x3 ret;
	ret.FillMat(m);

	double s = sin(angle);
	double c = cos(angle);

	ret.m[0] = c;	ret.m[3] = s;
	ret.m[1] = -s;	ret.m[4] = c;

	return ret;
}

Vector2		float3x3::GetTranslation()
{
	return Vector2((float)m[6], (float)m[7]);
}

/*Vector2		float3x3::GetRotation()
{
}*/