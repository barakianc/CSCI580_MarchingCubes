#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "Vertex.hpp"

class Model
{
public:
	// performance testing ONLY
	Vertex			*verts;
	Vertex			*faceVerts;
	unsigned long	numVerts;
	unsigned long	numFaces;

	unsigned int vaoId;	// vertex-array object
	unsigned int vboId;	// vertex-buffer object
	unsigned int nboId;
	unsigned int cboId;	// color-buffer object

	glm::mat4 modelMatrix;
	int modelMatrixLocation;

	Model()
	{
		vaoId = 0;
		vboId = nboId = cboId = 0;

		modelMatrix = glm::mat4();
		modelMatrixLocation = 0;
	}

	~Model()
	{
		delete [] verts;
		delete [] faceVerts;
	}
};

void LoadOBJModel(char* filename, Model &model)
{	
	if (filename == NULL)
		return;

	unsigned int numVerts = 0;
	unsigned int numFaces = 0;
	float *verts;
	int *faces;
	float *colors;

	vector<Vertex> tempVerts;

	ifstream inf;
	inf.open(filename);
	
	if (inf.fail())
	{
		cout << "Could not open the file!\n";
		return;
	}

	cout << "Loading OBJ file...\n";

	string str;
	glm::vec3 pos;
	while (!inf.eof())
	{
		inf >> str;

		if (str == "v")
		{
			Vertex v;
			inf >> v.coordinates.x >> v.coordinates.y >> v.coordinates.z;
			tempVerts.push_back(v);

			pos.x = pos.x + v.coordinates.x;
			pos.y = pos.y + v.coordinates.y;
			pos.z = pos.z + v.coordinates.z;
		}
	}
	pos = pos / (float)tempVerts.size();
	model.modelMatrix = glm::translate(model.modelMatrix, -pos);

	// temp data
	numVerts = tempVerts.size();
	verts = new float[numVerts * 3];
	colors = new float[numVerts * 3];
	faces = new int[3];

	// model data
	model.numVerts = tempVerts.size();
	model.verts = new Vertex[model.numVerts];

	for (unsigned int i=0; i<tempVerts.size(); i++)
	{
		// temp data
		model.verts[i] = tempVerts[i];

		// model data
		verts[i*3+0] = tempVerts[i].coordinates.x;
		verts[i*3+1] = tempVerts[i].coordinates.y;
		verts[i*3+2] = tempVerts[i].coordinates.z;
		colors[i*3+0] = colors[i*3+1] = colors[i*3+2] = 1.0;
	}

	cout << "done copying data...\n";

	glGenVertexArrays(1, &model.vaoId);
	glBindVertexArray(model.vaoId);

	glGenBuffers(1, &model.vboId);
	glBindBuffer(GL_ARRAY_BUFFER, model.vboId);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 3 * sizeof(float), verts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &model.cboId);
	glBindBuffer(GL_ARRAY_BUFFER, model.vboId);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 3 * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	delete [] verts;
	delete [] faces;
	delete [] colors;

	inf.close();
}

void LoadPLYModel(char* filename, Model &model)
{
	if (filename == NULL)
		return;

	unsigned int numVerts = 0;
	unsigned int numFaces = 0;
	float *verts;
	int *faces;
	float *colors;

	ifstream inf;
	inf.open(filename);
	
	if (inf.fail())
	{
		cout << "Could not open the file!\n";
		return;
	}

	string str;
	while (!inf.eof())
	{
		inf >> str;

		if (str == "element")
		{
			inf >> str;
			if (str == "vertex")
			{
				// temp data
				inf >> numVerts;
				verts = new float[numVerts * 3];
				colors = new float[numVerts * 4];

				// model data
				model.numVerts = numVerts;
				model.verts = new Vertex[model.numVerts];

				//cout << "numVerts = " << model.numVerts << endl;
			}
			else if (str == "face")
			{
				// temp data
				inf >> numFaces;
				faces = new int[numFaces * 3];

				// model data
				model.numFaces = numFaces;
				model.faceVerts = new Vertex[model.numFaces * 3];

				//cout << "numFaces = " << model.numFaces << endl;
			}
		}

		if (str == "end_header")
		{
			for (size_t i=0; i<numVerts; i++)
			{
				// model data
				inf >> model.verts[i].coordinates.x >> model.verts[i].coordinates.y >> model.verts[i].coordinates.z;
				inf >> str >> str;
				model.verts[i].color = Color(1,1,1,1);

				// temp data
				verts[i*3+0] = model.verts[i].coordinates.x;
				verts[i*3+1] = model.verts[i].coordinates.y;
				verts[i*3+2] = model.verts[i].coordinates.z;
				colors[i*4+0] = 1.0;
				colors[i*4+1] = 1.0;
				colors[i*4+2] = 1.0;
				colors[i*4+3] = 1.0;
			}

			// load faces
		}
	}

	glGenVertexArrays(1, &model.vaoId);
	glBindVertexArray(model.vaoId);

	glGenBuffers(1, &model.vboId);
	glBindBuffer(GL_ARRAY_BUFFER, model.vboId);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 3 * sizeof(float), verts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &model.cboId);
	glBindBuffer(GL_ARRAY_BUFFER, model.cboId);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 4 * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	delete [] verts;
	delete [] faces;
	delete [] colors;

	inf.close();
}

#endif