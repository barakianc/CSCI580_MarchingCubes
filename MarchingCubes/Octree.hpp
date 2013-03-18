#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

class Octree
{
public:
	enum
	{
		PXPYPZ,	// px py pz
		PXPYNZ,	// px py nz
		PXNYPZ,	// px ny pz
		PXNYNZ,	// px ny nz
		NXPYPZ,	// nx py pz
		NXPYNZ,	// nx py nz
		NXNYPZ,	// nx ny pz
		NXNYNZ,	// nx ny nz
		NUMSECTORS
	};

	Octree			*nodes;
	vector<Vertex>	points;
	Vector3			pos;
	Vector3			loCoord, hiCoord;
	float			dimensions;
	unsigned int	maxElements;
	int				level;

	static bool		drawLines;

	Octree()
	{
		nodes = NULL;
		pos = Vector3();
		loCoord = hiCoord = Vector3();
		dimensions = 0.0;
		maxElements = 100;
		level = 0;
	}

	~Octree()
	{
		Clear();
		delete [] nodes;
	}

	void AddElements(vector<Vertex> elements)
	{
		cout << "AddElements: elements.size=" << elements.size() << endl;
		for (size_t i=0; i<elements.size(); i++)
			points.push_back(elements[i]);
		GetDimensions();
		Process();
	}

	void GetDimensions()
	{
		if (this->level != 0)
			return;

		Vector3 loCoord, hiCoord;
		pos = Vector3();
		dimensions = 0.0;
		float maxDim = 0.0f;
		/*	iterate through all of the points in the model
			and find the dimensions of the model  */
		for (size_t i=0; i<points.size(); i++)
		{
			pos = pos + points[i].coordinates;
			if (abs(points[i].coordinates.x) > maxDim)
				maxDim = abs(points[i].coordinates.x);
			if (abs(points[i].coordinates.y) > maxDim)
				maxDim = abs(points[i].coordinates.y);
			if (abs(points[i].coordinates.z) > maxDim)
				maxDim = abs(points[i].coordinates.z);
			

			if (points[i].coordinates.x < loCoord.x)
				loCoord.x = points[i].coordinates.x;
			if (points[i].coordinates.y < loCoord.y)
				loCoord.y = points[i].coordinates.y;
			if (points[i].coordinates.z < loCoord.z)
				loCoord.z = points[i].coordinates.z;
			
			if (points[i].coordinates.x > hiCoord.x)
				hiCoord.x = points[i].coordinates.x;
			if (points[i].coordinates.y > hiCoord.y)
				hiCoord.y = points[i].coordinates.y;
			if (points[i].coordinates.z > hiCoord.z)
				hiCoord.z = points[i].coordinates.z;
		}
		//pos = pos / points.size();
		pos = (hiCoord+loCoord) * 0.5;
		dimensions = maxDim;
	}

	void Clear()
	{
		if (nodes == NULL)
			return;

		for (int i=0; i<8; i++)
		{
			nodes[i].Clear();
		}
	}

	void Process()
	{
		if (points.size() < maxElements)
			return;

		nodes = new Octree[8];
		float hDim = dimensions * 0.5f;
		float qDim = dimensions * 0.25f;
		nodes[PXPYPZ].pos = Vector3(pos.x + qDim, pos.y + qDim, pos.z + qDim);
		nodes[PXPYPZ].dimensions = hDim;
		nodes[PXPYNZ].pos = Vector3(pos.x + qDim, pos.y + qDim, pos.z - qDim);
		nodes[PXPYNZ].dimensions = hDim;
		nodes[PXNYPZ].pos = Vector3(pos.x + qDim, pos.y - qDim, pos.z + qDim);
		nodes[PXNYPZ].dimensions = hDim;
		nodes[PXNYNZ].pos = Vector3(pos.x + qDim, pos.y - qDim, pos.z - qDim);
		nodes[PXNYNZ].dimensions = hDim;
		nodes[NXPYPZ].pos = Vector3(pos.x - qDim, pos.y + qDim, pos.z + qDim);
		nodes[NXPYPZ].dimensions = hDim;
		nodes[NXPYNZ].pos = Vector3(pos.x - qDim, pos.y + qDim, pos.z - qDim);
		nodes[NXPYNZ].dimensions = hDim;
		nodes[NXNYPZ].pos = Vector3(pos.x - qDim, pos.y - qDim, pos.z + qDim);
		nodes[NXNYPZ].dimensions = hDim;
		nodes[NXNYNZ].pos = Vector3(pos.x - qDim, pos.y - qDim, pos.z - qDim);
		nodes[NXNYNZ].dimensions = hDim;

		
		// sort elements
		for (size_t i=0; i<points.size(); i++)
		{
			if (points[i].coordinates.x > pos.x)
			{
				if (points[i].coordinates.y > pos.y)
				{
					if (points[i].coordinates.z > pos.z)
						nodes[PXPYPZ].points.push_back(points[i]);
					else
						nodes[PXPYNZ].points.push_back(points[i]);
				}
				else
				{
					if (points[i].coordinates.z > pos.z)
						nodes[PXNYPZ].points.push_back(points[i]);
					else
						nodes[PXNYNZ].points.push_back(points[i]);
				}
			}
			else
			{
				if (points[i].coordinates.y > pos.y)
				{
					if (points[i].coordinates.z > pos.z)
						nodes[NXPYPZ].points.push_back(points[i]);
					else
						nodes[NXPYNZ].points.push_back(points[i]);
				}
				else
				{
					if (points[i].coordinates.z > pos.z)
						nodes[NXNYPZ].points.push_back(points[i]);
					else
						nodes[NXNYNZ].points.push_back(points[i]);
				}
			}
		}
		
		points.clear();
		for (int i=0; i<8; i++)
		{
			nodes[i].level = this->level + 1;
			nodes[i].Process();
		}
		//points.clear();
	}

	void Render()
	{
		if (this->drawLines)
		{
			double subDim = dimensions * 0.5f;
			glLineWidth(1);
			glColor4f(0.5,0.5,0.5,1);
			glBegin(GL_LINE_LOOP); // bottom square
				glVertex3d(pos.x - subDim, pos.y - subDim, pos.z - subDim);
				glVertex3d(pos.x + subDim, pos.y - subDim, pos.z - subDim);
				glVertex3d(pos.x + subDim, pos.y - subDim, pos.z + subDim);
				glVertex3d(pos.x - subDim, pos.y - subDim, pos.z + subDim);
			glEnd();
			glBegin(GL_LINE_LOOP); // top square
				glVertex3d(pos.x - subDim, pos.y + subDim, pos.z - subDim);
				glVertex3d(pos.x + subDim, pos.y + subDim, pos.z - subDim);
				glVertex3d(pos.x + subDim, pos.y + subDim, pos.z + subDim);
				glVertex3d(pos.x - subDim, pos.y + subDim, pos.z + subDim);
			glEnd();
			glBegin(GL_LINES);
				glVertex3d(pos.x - subDim, pos.y - subDim, pos.z - subDim);
				glVertex3d(pos.x - subDim, pos.y + subDim, pos.z - subDim);

				glVertex3d(pos.x + subDim, pos.y - subDim, pos.z - subDim);
				glVertex3d(pos.x + subDim, pos.y + subDim, pos.z - subDim);

				glVertex3d(pos.x + subDim, pos.y - subDim, pos.z + subDim);
				glVertex3d(pos.x + subDim, pos.y + subDim, pos.z + subDim);

				glVertex3d(pos.x - subDim, pos.y - subDim, pos.z + subDim);
				glVertex3d(pos.x - subDim, pos.y + subDim, pos.z + subDim);
			glEnd();
		}

		glPointSize(3);
		glBegin(GL_POINTS);
		for (size_t i=0; i<points.size(); i++)
		{
			glColor4d(points[i].color.r, points[i].color.g, points[i].color.b, points[i].color.a);
			glVertex3d(points[i].coordinates.x, points[i].coordinates.y, points[i].coordinates.z);
		}
		glEnd();

		if (nodes != NULL)
		{
			for (int i=0; i<8; i++)
				nodes[i].Render();
		}
	}
};

bool Octree::drawLines = true;

#endif