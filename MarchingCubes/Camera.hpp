#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm\glm.hpp>
#include <glm\gtx\projection.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "vector3.hpp"

class Plane
{
public:
	glm::vec3	normal;
	float		dist;

	Plane()
	{
		Plane(glm::vec3(0,1,0), 0);
	}

	Plane(glm::vec3 n, float d)
	{
		normal = n;
		dist = d;
	}

	bool inline PointInFront(glm::vec3 p)
	{
		if (glm::dot(normal, p) + dist > 0)
			return true;
		return false;
	}
};

class Frustum
{
public:
	Plane	*planes;

	enum
	{
		LEFT_PLANE,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		NEAR_PLANE,
		FAR_PLANE,
		NUM_PLANES
	};

	Frustum()
	{
		planes = new Plane[6];

		planes[LEFT_PLANE] = Plane(glm::vec3(1,0,0), 1);
		planes[RIGHT_PLANE] = Plane(glm::vec3(-1,0,0), 1);
		planes[TOP_PLANE] = Plane(glm::vec3(0,-1,0), 1);
		planes[BOTTOM_PLANE] = Plane(glm::vec3(0,1,0), 1);
		planes[NEAR_PLANE] = Plane(glm::vec3(0,0,1), 1);
		planes[FAR_PLANE] = Plane(glm::vec3(0,0,-1), 1);
	}

	~Frustum() {}

	bool PointInside(glm::vec3 p)
	{
		for (int i=0; i<6; i++)
		{
			if (!planes[i].PointInFront(p))
				return false;
		}
		return true;
	}
};

class Camera
{
public:
	glm::vec3	pos;
	glm::vec3	lookAt;
	glm::vec3	up;
	double		pitchDegrees;
	double		yawDegrees;

	int			width, height;
	float		ratio;
	float		fovx, fovy;
	float		nearDist, farDist;

	glm::mat4	projMatrix;
	glm::mat4	viewMatrix;
	int	projMatrixLocation;
	int viewMatrixLocation;

	Camera()
	{
		pitchDegrees = 0.0;
		yawDegrees = 90.0;

		width = 1280;
		height = 720;
		ratio = (float)width / (float)height;
		fovy = 90;
		fovx = fovy * ratio;
		nearDist = 0.1f;
		farDist = 1000.0f;

		projMatrixLocation = viewMatrixLocation = 0;
	}

	void StrafeCamera(float d) // left and right
	{
		glm::vec3 right = glm::normalize(glm::cross(lookAt - pos, up));

		pos += right * d;
		lookAt += right * d;

		BuildViewMatrix();
		cout << pos.x << " " << pos.y << " " << pos.z << "   " << lookAt.x << " " << lookAt.y << " " << lookAt.z << endl;
	}

	void DollyCamera(float d) // forward and backward
	{
		glm::vec3 view = glm::normalize(lookAt - pos);

		pos += view * d;
		lookAt += view * d;

		BuildViewMatrix();
		cout << pos.x << " " << pos.y << " " << pos.z << "   " << lookAt.x << " " << lookAt.y << " " << lookAt.z << endl;
	}

	void BoomCamera(float d)
	{
		pos.y += d;
		lookAt.y += d;

		BuildViewMatrix();
	}

	void AdjustPitch(float deg)
	{
		pitchDegrees += deg;

		if (pitchDegrees > 90)
			pitchDegrees = 89.5;
		if (pitchDegrees < -90)
			pitchDegrees = -89.5;
		
		lookAt.x = (float)cos(yawDegrees*PIOVER180) * (float)cos(pitchDegrees * PIOVER180);
		lookAt.y = (float)sin(pitchDegrees*PIOVER180);
		lookAt.z = (float)sin(yawDegrees*PIOVER180) * (float)cos(pitchDegrees * PIOVER180);
		
		lookAt = glm::normalize(lookAt) + pos;
		
		BuildViewMatrix();
		cout << pos.x << " " << pos.y << " " << pos.z << "   " << lookAt.x << " " << lookAt.y << " " << lookAt.z << endl;
	}

	void AdjustYaw(float deg)
	{
		yawDegrees += deg;

		lookAt.x = (float)cos(yawDegrees*PIOVER180) * (float)cos(pitchDegrees * PIOVER180);
		lookAt.y = (float)sin(pitchDegrees*PIOVER180);
		lookAt.z = (float)sin(yawDegrees*PIOVER180) * (float)cos(pitchDegrees * PIOVER180);

		lookAt = glm::normalize(lookAt) + pos;

		BuildViewMatrix();
		cout << pos.x << " " << pos.y << " " << pos.z << "   " << lookAt.x << " " << lookAt.y << " " << lookAt.z << endl;
	}

	void RotY(float deg)
	{
		yawDegrees += deg;

		glm::vec3 l = glm::normalize(pos - lookAt);

		float mag = (float)l.length();
		pos.x = lookAt.x + (float)cos(yawDegrees*PIOVER180)*mag;
		pos.z = lookAt.z + (float)sin(yawDegrees*PIOVER180)*mag;
		
		BuildViewMatrix();
	}

	void GetFrustum(Frustum &f)
	{
		// update matrices
		BuildProjMatrix();
		BuildViewMatrix();
		
		// calculate verts on frustum
		glm::vec3 view = glm::vec3(viewMatrix[2][0]*farDist-pos.x,
									viewMatrix[2][1]*farDist-pos.y,
									viewMatrix[2][2]*farDist-pos.z);
		glm::vec3 up = glm::vec3(viewMatrix[1][0], viewMatrix[1][1], viewMatrix[1][2]);
		glm::vec3 right = glm::vec3(viewMatrix[0][0], viewMatrix[0][1], viewMatrix[0][2]);
		float hFar = view.length() * (float)atan(fovy * 0.5);
		float wFar = view.length() * (float)atan(fovx * 0.5);
		float hNear = (view.length() - nearDist) * (float)atan(fovy * 0.5);
		float wNear = (view.length() - nearDist) * (float)atan(fovx * 0.5);

		// far points
		glm::vec3 ftl = view + (up * hFar) - (right * wFar);
		glm::vec3 ftr = view + (up * hFar) + (right * wFar);
		glm::vec3 fbl = view - (up * hFar) - (right * wFar);
		glm::vec3 fbr = view - (up * hFar) + (right * wFar);

		// near points		
		glm::vec3 ntl = view + (up * hNear) - (right * wNear);
		glm::vec3 ntr = view + (up * hNear) + (right * wNear);
		glm::vec3 nbl = view - (up * hNear) - (right * wNear);
		glm::vec3 nbr = view - (up * hNear) + (right * wNear);

		// DEBUG output verts
		/*cout << "ftl: " << ftl[0] << " " << ftl[1] << " " << ftl[2] << endl;
		cout << "ftr: " << ftr[0] << " " << ftr[1] << " " << ftr[2] << endl;
		cout << "fbl: " << fbl[0] << " " << fbl[1] << " " << fbl[2] << endl;
		cout << "fbr: " << fbr[0] << " " << fbr[1] << " " << fbr[2] << endl;
		
		cout << "ntl: " << ntl[0] << " " << ntl[1] << " " << ntl[2] << endl;
		cout << "ntr: " << ntr[0] << " " << ntr[1] << " " << ntr[2] << endl;
		cout << "nbl: " << nbl[0] << " " << nbl[1] << " " << nbl[2] << endl;
		cout << "nbr: " << nbr[0] << " " << nbr[1] << " " << nbr[2] << endl;*/
	}

	void BuildProjMatrix()
	{
		ratio = (float)width / (float)height;
		fovx = ratio * fovy;
		projMatrix = glm::perspective<float>(fovy, ratio, nearDist, farDist);
	}

	void OutputMatrix()
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
				cout << viewMatrix[j][i] << " ";
			cout << endl;
		}
	}

private:

	inline void BuildViewMatrix()
	{
		viewMatrix = glm::lookAt(pos, lookAt, up);
	}
};

#endif