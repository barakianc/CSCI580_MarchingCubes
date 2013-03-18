#include "vector3.hpp"

//const float piOver180 = 3.1416/180.0;

Vector3::Vector3()
{
	x = y = z = 0.0;
}

Vector3::Vector3(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}

Vector3::Vector3(float angle)
{
	x = (float)cos(angle * PIOVER180);
	y = (float)sin(angle * PIOVER180);
	z = 0;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-(Vector3 vector) const
{ 
	return Vector3( x - vector.x, y - vector.y, z - vector.z);
}

Vector3 Vector3::operator-(float scalar)
{
	Vector3 temp(x, y, z);
	float mag = sqrt(x*x + y*y + z*z);
	temp.x /= mag;
	temp.y /= mag;
	temp.z /= mag;

	temp.x *= scalar;
	temp.y *= scalar;
	temp.z *= scalar;

	return temp;
}

Vector3 Vector3::operator+(Vector3 vector)
{ 
	return Vector3( x + vector.x, y + vector.y, z + vector.z);
}

Vector3 Vector3::operator+(float scalar)
{
	Vector3 temp(x, y, z);
	float mag = sqrt(x*x + y*y + z*z);
	if(mag == 0) mag=0.0001f;

	temp.x /= mag;
	temp.y /= mag;
	temp.z /= mag;

	temp.x *= scalar;
	temp.y *= scalar;
	temp.z *= scalar;

	return temp;
}

Vector3 Vector3::operator*(Vector3 vector)
{ 
	return Vector3( x * vector.x, y * vector.y, z * vector.z);
}

Vector3 Vector3::operator*(float scalar)
{ 
	return Vector3( x*scalar, y*scalar, z*scalar);
}

Vector3 Vector3::operator/(Vector3 vector)
{ 
	return Vector3( x/vector.x, y/vector.y, z/vector.z);
}

Vector3 Vector3::operator/(float scalar)
{ 
	return Vector3( x/scalar, y/scalar, z/scalar);
}

bool Vector3::operator<(Vector3 vector)
{
	if((x < vector.x) && (y < vector.y) && (z < vector.z))
		return true;
	else
		return false;
}

bool Vector3::operator>(Vector3 vector)
{
	if((x > vector.x) && (y > vector.y) && (z > vector.z))
		return true;
	else
		return false;
}

bool Vector3::operator<=(Vector3 vector)
{
	if((x <= vector.x) && (y <= vector.y) && (z <= vector.z))
		return true;
	else
		return false;
}

bool Vector3::operator>=(Vector3 vector)
{
	if((x >= vector.x) && (y >= vector.y) && (z >= vector.z))
		return true;
	else
		return false;
}

bool Vector3::operator==(Vector3 vector)
{
	if((vector.x == x) && (vector.y == y) && (vector.z == z))
		return true;
	else
		return false;
}

bool Vector3::operator!=(Vector3 vector)
{
	if((vector.x == x) && (vector.y == y) && (vector.z == z))
		return false;
	else
		return true;
}

Vector3 &Vector3::operator=(const Vector3 & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return *this;
}

/*inline const float *Vector3::v()
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
	return V;
}*/

Vector3	CreateNormal(Vector3 v1, Vector3 v2, Vector3 v3)
{
	v1 = Normalize(v1 - v2);
	v2 = Normalize(v3 - v2);
	return Cross(v1, v2);
}

Vector3	CreateNormal(Vector3 *v)
{
	return Cross(Normalize(v[0]-v[1]), Normalize(v[2]-v[1]));
}

Vector3 Cross(Vector3 v1, Vector3 v2)
{
	Vector3	temp_vector;

	temp_vector.x = (v1.y * v2.z) - (v1.z * v2.y);
	temp_vector.y = (-v1.x * v2.z) + (v1.z * v2.x);
	temp_vector.z = (v1.x * v2.y) - (v1.y * v2.x);

	return temp_vector;
}

float InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating value
	i = 0x5f3759df - (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits back to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}

float Magnitude(Vector3 vector)
{	
	return sqrt(vector.x * vector.x +
				vector.y * vector.y +
				vector.z * vector.z);
}

float Magnitude(float x, float y, float z)
{
	return Magnitude(Vector3(x,y,z));
}

/*float Magnitude(float x, float y)
{	
	float val = sqrt(x*x + y*y);

	return val;
}*/

Vector3 Normalize(Vector3 vector)
{
	/*float len = Magnitude(vector);
	vector.x /= len;
	vector.y /= len;
	vector.z /= len;*/

	double mag = Magnitude(vector);
	
	if( mag <= 0.00001 )
	{
		mag = 0.00001;
	}

	vector = vector / Magnitude(vector);

	return vector;
}

Vector3 Normalize(float x, float y, float z)
{
	Vector3 vector(x, y, z);

	/*float len = Magnitude(vector);
	x /= len;
	y /= len;
	z /= len;
	vector.x = x; vector.y = y; vector.z = z;*/

	vector = vector / Magnitude(vector);

	return vector;
}

float Dot(Vector3 vector1, Vector3 vector2)
{
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

float AngleBetweenVectors(Vector3 vector1, Vector3 vector2)
{
	//returns the angle in radians
	return acos(Dot(Normalize(vector1),Normalize(vector2)));
}

float AngleBetweenVectorsD(Vector3 vector1, Vector3 vector2)
{
	//returns the angle in degrees
	return (float)acos(Dot(Normalize(vector1),Normalize(vector2)))/(float)PIOVER180;
}

float PlaneDist(Vector3 vector1, Vector3 vector2)
{
	return -((vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z));
}

Vector3 Lerp(Vector3 v1, Vector3 v2, float k)
{
	return Vector3(((v2-v1) * k ) + v1);
}

Vector3 Reflect(Vector3 n, Vector3 v)
{	
	return v - (n * Dot(v, n) * 2.0);
}