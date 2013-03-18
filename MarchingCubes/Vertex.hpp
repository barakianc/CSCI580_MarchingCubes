#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__

#include "vector2.hpp"
#include "vector3.hpp"
#include "Color.hpp"


struct Vertex
{
	Vector3			coordinates;
	Vector3			normal;
	Color			color;
	Vector2			uv;

	Vertex()
	{
		coordinates = Vector3();
		normal = Vector3(0,1,0);
		uv = Vector2();
		color.r = color.g = color.b = color.a = 1.0f;
	}

	Vertex(const Vertex &v)
	{
		this->coordinates = v.coordinates;
		this->uv = v.uv;
		this->color = v.color;
	}

	Vertex &Vertex::operator=(const Vertex & v)
	{
		this->coordinates = v.coordinates;
		this->color = v.color;
		this->uv = v.uv;

		return *this;
	}

	virtual ~Vertex()
	{
	}
};


//const unsigned int COORD_OFFSET = 0;
//const unsigned int NORMAL_OFFSET = COORD_OFFSET + sizeof(Vector3);
//const unsigned int COLOR_OFFSET = NORMAL_OFFSET + sizeof(Vector3);
//const unsigned int UV_OFFSET = COLOR_OFFSET + sizeof(Color);

#endif