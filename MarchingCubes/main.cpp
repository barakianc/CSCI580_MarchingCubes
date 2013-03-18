#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <Windows.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL\freeglut_ext.h>
#include <GL\freeglut_std.h>
#include <GL\glut.h>

#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

#include <Cg\cg.h>
#include <Cg\cgGL.h>

#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cggl.lib")

#include <glm\glm.hpp>
#include <glm\gtx\projection.hpp>
#include <glm\gtc\matrix_transform.hpp>


#include "XboxController.hpp"
#include "Shader.hpp"
#include "Timer.hpp"
#include "FPSCounter.hpp"
#include "float3x3.h"
#include "float4x4.h"
#include "vector3.hpp"
#include "Vertex.hpp"
#include "Font.h"
#include "Model.hpp"
#include "Octree.hpp"
#include "Camera.hpp"
#include "Histogram.hpp"

#include "Perlin_Noise.h"
#include "MarchingCubes.hpp"

using namespace std;

int screenWidth = 1280;
int screenHeight = 720;


XboxController controller;

Shader basicShader;
Shader lightShader;

bool	sp;

Font font;
GLuint tex;

Model cube;
float cubeVerts[] = 
{
	// front
	-0.5, -0.5, -0.5, 
	 0.5, -0.5,	-0.5, 
	 0.5,  0.5,	-0.5,
	 
	-0.5, -0.5, -0.5, 
	 0.5,  0.5,	-0.5,
	-0.5,  0.5,	-0.5,
	
	// back
	 0.5, -0.5,	 0.5, 
	-0.5, -0.5,  0.5, 
	-0.5,  0.5,	 0.5, 
	
	 0.5, -0.5,	 0.5, 
	-0.5,  0.5,	 0.5, 
	 0.5,  0.5,	 0.5, 

	 // right
	 0.5, -0.5,  0.5,
	 0.5,  0.5, -0.5,
	 0.5, -0.5, -0.5,

	 0.5, -0.5,  0.5,
	 0.5,  0.5, -0.5,
	 0.5,  0.5,  0.5,

	// left
	-0.5,-0.5,  0.5,
	-0.5,-0.5, -0.5,
	-0.5, 0.5, -0.5,
	 
	-0.5,-0.5,  0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5,  0.5,

	// top
	-0.5, 0.5, -0.5,
	 0.5, 0.5, -0.5,
	 0.5, 0.5,  0.5,

	-0.5, 0.5, -0.5,
	 0.5, 0.5,  0.5,
	-0.5, 0.5,  0.5,

	// bottom
	-0.5,-0.5, 0.5, 
	 0.5,-0.5, 0.5,
	 0.5,-0.5,-0.5,
	 
	-0.5,-0.5, 0.5,
	 0.5,-0.5,-0.5,
	-0.5,-0.5,-0.5
};

float cubeColors[] = 
{
	// front
	0.0,0.0,0.0,1.0,
	1.0,0.0,0.0,1.0, 
	1.0,1.0,0.0,1.0,
	 
	0.0,0.0,0.0,1.0,
	1.0,1.0,0.0,1.0,
	0.0,1.0,0.0,1.0,
	
	// back
	1.0,0.0,1.0,1.0,
	0.0,0.0,1.0,1.0,
	0.0,1.0,1.0,1.0,
	
	1.0,0.0,1.0,1.0,
	0.0,1.0,1.0,1.0,
	1.0,1.0,1.0,1.0,

	// right
	1.0,0.0,1.0,1.0,
	1.0,0.0,0.0,1.0,
	1.0,1.0,0.0,1.0,

	1.0,0.0,1.0,1.0,
	1.0,1.0,0.0,1.0,
	1.0,1.0,1.0,1.0,

	// left
	0.0,0.0,1.0,1.0,
	0.0,0.0,0.0,1.0,
	0.0,1.0,0.0,1.0,
	 
	0.0,0.0,1.0,1.0,
	0.0,1.0,0.0,1.0,
	0.0,1.0,1.0,1.0,

	// top
	0.0,1.0,0.0,1.0,
	1.0,1.0,0.0,1.0,
	1.0,1.0,1.0,1.0,

	0.0,1.0,0.0,1.0,
	1.0,1.0,1.0,1.0,
	0.0,1.0,1.0,1.0,

	// bottom
	0.0,0.0,1.0,1.0,
	1.0,0.0,1.0,1.0,
	1.0,0.0,0.0,1.0,
	 
	0.0,0.0,1.0,1.0,
	1.0,0.0,0.0,1.0,
	0.0,0.0,0.0,1.0
};

float cubeNormals[] = 
{
	// front
	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,

	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,

	// back
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	// right
	1.0f, 0.0f, 0.0f, 
	1.0f, 0.0f, 0.0f, 
	1.0f, 0.0f, 0.0f, 

	1.0f, 0.0f, 0.0f, 
	1.0f, 0.0f, 0.0f, 
	1.0f, 0.0f, 0.0f, 

	// left
	-1.0f, 0.0f, 0.0f, 
	-1.0f, 0.0f, 0.0f, 
	-1.0f, 0.0f, 0.0f, 

	-1.0f, 0.0f, 0.0f, 
	-1.0f, 0.0f, 0.0f, 
	-1.0f, 0.0f, 0.0f, 

	// top
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	// bottom
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f
};
void BuildCube()
{
	glGenVertexArrays(1, &cube.vaoId);
	glBindVertexArray(cube.vaoId);

	cube.numVerts = 36;
	cube.numFaces = 12;
	cube.verts = new Vertex[36];

	glGenBuffers(1, &cube.vboId);
	glBindBuffer(GL_ARRAY_BUFFER, cube.vboId);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), cubeVerts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray((GLuint)0);
	
	glGenBuffers(1, &cube.cboId);
	glBindBuffer(GL_ARRAY_BUFFER, cube.cboId);
	glBufferData(GL_ARRAY_BUFFER, 36 * 4 * sizeof(float), cubeColors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray((GLuint)1);

	/*glGenBuffers(1, &cube.nboId);
	glBindBuffer(GL_ARRAY_BUFFER, cube.nboId);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), cubeNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);*/

	glBindVertexArray(0);

	//cube.modelMatrix = glm::translate(cube.modelMatrix, glm::vec3(2.25, 0, 0));
	cube.modelMatrix = glm::mat4();

	//delete [] verts;
	//delete [] colors;
}

Camera camera;
Octree octree;
Model bunny;

Model bear;


MarchingCubesGenerator meshGenerator(150);
GLuint meshVaoId;
GLuint meshVboId;
GLuint meshNboId;
GLuint meshCboId;

GLuint noiseVaoId;
GLuint noiseVboId;
GLuint noiseCboId;
GLuint numNoisePoints;

Model cubeCases[256];
void BuildCubeCases(MarchingCubesGenerator meshGenerator)
{
	vector<Vertex> tempVerts;

	for (int i=0; i<256; i++)
	{
		cubeCases[i].modelMatrix = glm::mat4();

		// iterate through number of fans in cube
		for (int j=0; j<meshGenerator.lookUpFans[i]; j++)
		{
			// iterate through 6 possible triangles in the fan
			vector<Vertex> tempFan;
			Vertex v;
			for (int k=0; k<6; k++)
			{
				if(meshGenerator.lookUpTable[i][j][k][0] != -1)
				{
					v.coordinates = Vector3((float)meshGenerator.lookUpTable[i][j][k][0],
											(float)meshGenerator.lookUpTable[i][j][k][1],
											(float)meshGenerator.lookUpTable[i][j][k][2]);
					v.normal = Vector3((float)meshGenerator.lookUpNormTable[i][j][k][0],
										(float)meshGenerator.lookUpNormTable[i][j][k][1],
										(float)meshGenerator.lookUpNormTable[i][j][k][2]);
					tempFan.push_back(v);
				}
				else
				{
					// convert triangle-fan to individual triangles
					for (unsigned int m=1; m<tempFan.size()-1; m++)
					{
						// first vert
						v.coordinates = tempFan[0].coordinates;
						v.normal = tempFan[0].normal;
						tempVerts.push_back(v);

						// second vert						
						v.coordinates = tempFan[m].coordinates;
						v.normal = tempFan[m].normal;
						tempVerts.push_back(v);

						// third vert	
						v.coordinates = tempFan[m+1].coordinates;
						v.normal = tempFan[m+1].normal;						
						tempVerts.push_back(v);
					}
					break;
				}
			}
			tempFan.clear();
		}
		
		cubeCases[i].numVerts = tempVerts.size();
		cubeCases[i].numFaces = tempVerts.size() / 3;

		cubeCases[i].verts = new Vertex[cubeCases[i].numVerts];
		for (unsigned long j=0; j<cubeCases[i].numVerts; j++)
		{
			cubeCases[i].verts[j].coordinates = tempVerts[j].coordinates;
			cubeCases[i].verts[j].normal = tempVerts[j].normal;
			
			switch(j%3)
			{
			case 0:
				cubeCases[i].verts[j].color = Color(1.0, 0.0, 0.0, 1.0);
				break;
			case 1:
				cubeCases[i].verts[j].color = Color(0.0, 1.0, 0.0, 1.0);
				break;
			case 2:
				cubeCases[i].verts[j].color = Color(0.0, 0.0, 1.0, 1.0);
				break;
			}
			/*cubeCases[i].verts[j].color.r = 0.5f;
			cubeCases[i].verts[j].color.g = 0.5f;
			cubeCases[i].verts[j].color.b = 1.0f;*/
			cubeCases[i].verts[j].color.a = 1.0f;
		}
		tempVerts.clear();

		float *verts = new float[cubeCases[i].numVerts * 3];
		float *normals = new float[cubeCases[i].numVerts * 3];
		float *colors = new float[cubeCases[i].numVerts * 4];
		for (unsigned int j=0; j < cubeCases[i].numVerts; j++)
		{
			verts[j*3+0] = cubeCases[i].verts[j].coordinates.x;
			verts[j*3+1] = cubeCases[i].verts[j].coordinates.y;
			verts[j*3+2] = cubeCases[i].verts[j].coordinates.z;

			normals[j*3+0] = cubeCases[i].verts[j].normal.x;
			normals[j*3+1] = cubeCases[i].verts[j].normal.y;
			normals[j*3+2] = cubeCases[i].verts[j].normal.z;

			colors[j*4+0] = cubeCases[i].verts[j].color.r;
			colors[j*4+1] = cubeCases[i].verts[j].color.g;
			colors[j*4+2] = cubeCases[i].verts[j].color.b;
			colors[j*4+3] = cubeCases[i].verts[j].color.a;
		}
		glGenVertexArrays(1, &cubeCases[i].vaoId);
		glBindVertexArray(cubeCases[i].vaoId);

		glGenBuffers(1, &cubeCases[i].vboId);
		glBindBuffer(GL_ARRAY_BUFFER, cubeCases[i].vboId);
		glBufferData(GL_ARRAY_BUFFER, cubeCases[i].numVerts * 3 * sizeof(float), verts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray((GLuint)0);

		glGenBuffers(1, &cubeCases[i].cboId);
		glBindBuffer(GL_ARRAY_BUFFER, cubeCases[i].cboId);
		glBufferData(GL_ARRAY_BUFFER, cubeCases[i].numVerts * 4 * sizeof(float), colors, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray((GLuint)1);

		glGenBuffers(1, &cubeCases[i].nboId);
		glBindBuffer(GL_ARRAY_BUFFER, cubeCases[i].nboId);
		glBufferData(GL_ARRAY_BUFFER, cubeCases[i].numVerts * 3 * sizeof(float), normals, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray((GLuint)2);

		delete [] verts;
		delete [] normals;
		delete [] colors;
	}
}

void DebugDrawCubeCases()
{
	basicShader.Bind();
	for (int i=0; i<256; i++)
	{
		glm::mat4 pos;
		pos = glm::translate(pos, glm::vec3(i%16, i/16, 0));

		glBindVertexArray(cubeCases[i].vaoId);
			glUniformMatrix4fv(camera.projMatrixLocation, 1, GL_FALSE, &camera.projMatrix[0][0]);
			glUniformMatrix4fv(camera.viewMatrixLocation, 1, GL_FALSE, &camera.viewMatrix[0][0]);
			glUniformMatrix4fv(cube.modelMatrixLocation, 1, GL_FALSE, &pos[0][0]);
			//glDrawArrays(GL_POINTS, 0, cubeCases[i].numVerts);
			glDrawArrays(GL_TRIANGLES, 0, cubeCases[i].numVerts);
		glBindVertexArray(0);
	}
	basicShader.UnBind();
}

void BuildMesh(MarchingCubesGenerator &meshGenerator)
{
	vector<Vertex> tempVerts;
	vector<Vertex> fanVerts;
	Vertex v;

	for (int i=0; i<meshGenerator.numCubes-1; i++)
	{
		for (int j=0; j<meshGenerator.numCubes-1; j++)
		{
			for (int k=0; k<meshGenerator.numCubes-1; k++)
			{
				int vertFormat = meshGenerator.cubes[i][j][k].vertformat;
								
				for (unsigned long m=0; m<cubeCases[vertFormat].numVerts; m++)
				{
					v = cubeCases[vertFormat].verts[m];
					v.coordinates = v.coordinates + Vector3((float)i,(float)j,(float)k);
					v.normal = Vector3(meshGenerator.allvertNormals[i][j][k].n1,
										meshGenerator.allvertNormals[i][j][k].n2,
										meshGenerator.allvertNormals[i][j][k].n3);
					tempVerts.push_back(v);
					
					//cout << i << "  [" << v.normal.x << " " << v.normal.y << " " << v.normal.z << "]\n";
				}
			}
		}
	}

	meshGenerator.numVerts = tempVerts.size();
	meshGenerator.numTriangles = tempVerts.size() / 3;
	float *verts = new float[meshGenerator.numVerts * 3];
	float *normals = new float[meshGenerator.numVerts * 3];
	float *vertColors = new float[meshGenerator.numVerts * 4];
	for (unsigned int i=0; i<meshGenerator.numVerts; i++)
	{
		verts[i*3+0] = tempVerts[i].coordinates.x;
		verts[i*3+1] = tempVerts[i].coordinates.y;
		verts[i*3+2] = tempVerts[i].coordinates.z;

		normals[i*3+0] = tempVerts[i].normal.x;
		normals[i*3+1] = tempVerts[i].normal.y;
		normals[i*3+2] = tempVerts[i].normal.z;
		
		vertColors[i*4+0] = tempVerts[i].color.r;
		vertColors[i*4+1] = tempVerts[i].color.g;
		vertColors[i*4+2] = tempVerts[i].color.b;
		vertColors[i*4+3] = tempVerts[i].color.a;
	}
	cout << "meshGenerator.numVerts = " << meshGenerator.numVerts << endl;
	cout << "meshGenerator.numTriangles = " << meshGenerator.numTriangles << endl;

	glGenVertexArrays(1, &meshVaoId);
	glBindVertexArray(meshVaoId);

	glGenBuffers(1, &meshVboId);
	glBindBuffer(GL_ARRAY_BUFFER, meshVboId);
	glBufferData(GL_ARRAY_BUFFER, meshGenerator.numVerts * 3 * sizeof(float), verts, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray((GLuint)0);
	
	glGenBuffers(1, &meshNboId);
	glBindBuffer(GL_ARRAY_BUFFER, meshNboId);
	glBufferData(GL_ARRAY_BUFFER, meshGenerator.numVerts * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray((GLuint)2);

	glGenBuffers(1, &meshCboId);
	glBindBuffer(GL_ARRAY_BUFFER, meshCboId);
	glBufferData(GL_ARRAY_BUFFER, meshGenerator.numVerts * 4 * sizeof(float), vertColors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray((GLuint)1);

	delete [] verts;
	delete [] vertColors;
}

void HandleControllerInput(XboxController controller)
{	
	controller.Update();

	if (controller.rightThumbStickX > 0)
		camera.AdjustYaw(controller.rightThumbStickX * 1.0f);
	else if (controller.rightThumbStickX < 0)
		camera.AdjustYaw(controller.rightThumbStickX * 1.0f);

	if (controller.rightThumbStickY < 0)
		camera.AdjustPitch(controller.rightThumbStickY * 1.0f);
	else if (controller.rightThumbStickY > 0)
		camera.AdjustPitch(controller.rightThumbStickY * 1.0f);

	if (controller.leftThumbStickY > 0)
		camera.DollyCamera(controller.leftThumbStickY * 0.1f);
	if (controller.leftThumbStickY < 0)
		camera.DollyCamera(controller.leftThumbStickY * 0.1f);
	
	if (controller.leftThumbStickX > 0)
		camera.StrafeCamera(controller.leftThumbStickX * 0.1f);
	if (controller.leftThumbStickX < 0)
		camera.StrafeCamera(controller.leftThumbStickX * 0.1f);

	if (controller.leftTrigger > 0)
		camera.BoomCamera(controller.leftTrigger * -0.0005f);
	if (controller.rightTrigger > 0)
		camera.BoomCamera(controller.rightTrigger * 0.0005f);
}

void render()
{
	// this is here because of a limitation with GLUT
	HandleControllerInput(controller);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z,
			camera.lookAt.x, camera.lookAt.y, camera.lookAt.z,
			camera.up.x, camera.up.y, camera.up.z);

	glBegin(GL_LINES);
		glColor4f(1,0,0,1); glVertex3f(0,0,0); glVertex3f(5,0,0);
		glColor4f(0,1,0,1); glVertex3f(0,0,0); glVertex3f(0,5,0);
		glColor4f(0,0,1,1); glVertex3f(0,0,0); glVertex3f(0,0,5);
	glEnd();

	// test cube
	/*basicShader.Bind();
	glBindVertexArray(cube.vaoId);
			glUniformMatrix4fv(camera.projMatrixLocation, 1, GL_FALSE, &camera.projMatrix[0][0]);
			glUniformMatrix4fv(camera.viewMatrixLocation, 1, GL_FALSE, &camera.viewMatrix[0][0]);
			glUniformMatrix4fv(cube.modelMatrixLocation, 1, GL_FALSE, &cube.modelMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, cube.numFaces);			
		glBindVertexArray(0);
	basicShader.UnBind();*/

	// point cloud
	/*glPointSize(2);
	basicShader.Bind();
		glBindVertexArray(noiseVaoId);
			glUniformMatrix4fv(camera.projMatrixLocation, 1, GL_FALSE, &camera.projMatrix[0][0]);
			glUniformMatrix4fv(camera.viewMatrixLocation, 1, GL_FALSE, &camera.viewMatrix[0][0]);
			glUniformMatrix4fv(cube.modelMatrixLocation, 1, GL_FALSE, &cube.modelMatrix[0][0]);
			glDrawArrays(GL_POINTS, 0, numNoisePoints);			
		glBindVertexArray(0);
	basicShader.UnBind();*/

	//DebugDrawCubeCases();
	//DrawMarchingCubesMesh(meshGenerator);
	/*lightShader.Bind();
		glBindVertexArray(meshVaoId);
			glUniformMatrix4fv(camera.projMatrixLocation, 1, GL_FALSE, &camera.projMatrix[0][0]);
			glUniformMatrix4fv(camera.viewMatrixLocation, 1, GL_FALSE, &camera.viewMatrix[0][0]);
			glUniformMatrix4fv(cube.modelMatrixLocation, 1, GL_FALSE, &cube.modelMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, meshGenerator.numTriangles);
		glBindVertexArray(0);
	lightShader.UnBind();*/

	basicShader.Bind();
		glBindVertexArray(bear.vaoId);
			glUniformMatrix4fv(camera.projMatrixLocation, 1, GL_FALSE, &camera.projMatrix[0][0]);
			glUniformMatrix4fv(camera.viewMatrixLocation, 1, GL_FALSE, &camera.viewMatrix[0][0]);
			glUniformMatrix4fv(bear.modelMatrixLocation, 1, GL_FALSE, &bear.modelMatrix[0][0]);
			glDrawArrays(GL_POINTS, 0, bear.numVerts);
		glBindVertexArray(0);
	basicShader.UnBind();


	FPSCounter::update();
	char title[256] = "";
	sprintf_s(title, sizeof(char)*256, "Marching Cubes  FPS: %i", FPSCounter::getFPS());
	glutSetWindowTitle(title);

	glutSwapBuffers();
}

void keyboardSpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		//camera.RotY(5.0);
		camera.AdjustYaw(-5.0);
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		//camera.RotY(-5.0);
		camera.AdjustYaw(5.0);
	}

	if (key == GLUT_KEY_UP)
	{
		camera.AdjustPitch(5.0);
	}
	else if (key == GLUT_KEY_DOWN)
	{
		camera.AdjustPitch(-5.0);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == (int)27)
	{
		exit(0);
	}

	if (key == 'o')
	{
		octree.drawLines = !octree.drawLines;
	}

	if (key == 'w' || key == 'W')
	{
		camera.DollyCamera(0.5);
	}
	if (key == 's' || key == 'S')
	{
		camera.DollyCamera(-0.5);
	}
	if (key == 'a' || key == 'A')
	{
		camera.StrafeCamera(-1.0);
	}
	if (key == 'd' || key == 'D')
	{
		camera.StrafeCamera(1.0);
	}

	if (key == '-')
	{
		camera.BoomCamera(-1.0);
	}
	if (key == '=')
	{
		camera.BoomCamera(1.0);
	}
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	float ratio = (float)width / (float)height;
	
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, width, height);
		gluPerspective(90, ratio, 0, 1000);
	glMatrixMode(GL_MODELVIEW);

	camera.projMatrix = glm::perspective<float>(90.0f, ratio, 0.1f, 1000.0f);
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glewInit();

	const GLubyte *str = glGetString(GL_VERSION);
	cout << "GL Version: " << str << endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// shader setup
	basicShader.Init("basic.vert", "basic.frag");
	camera.projMatrixLocation = glGetUniformLocation(basicShader.progId, "projMatrix");
	camera.viewMatrixLocation = glGetUniformLocation(basicShader.progId, "viewMatrix");
	bunny.modelMatrixLocation = glGetUniformLocation(basicShader.progId, "modelMatrix");
	cube.modelMatrixLocation = glGetUniformLocation(basicShader.progId, "modelMatrix");

	lightShader.Init("light.vert", "light.frag");
	camera.projMatrixLocation = glGetUniformLocation(lightShader.progId, "projMatrix");
	camera.viewMatrixLocation = glGetUniformLocation(lightShader.progId, "viewMatrix");
	bunny.modelMatrixLocation = glGetUniformLocation(lightShader.progId, "modelMatrix");
	cube.modelMatrixLocation = glGetUniformLocation(lightShader.progId, "modelMatrix");

	LoadOBJModel("bear.obj", bear);
	
	// generate mesh
	/*meshGenerator.initCubes();
	meshGenerator.initFans();
	BuildCubeCases(meshGenerator);
	BuildMesh(meshGenerator);*/

	BuildCube();

	// set up camera
	camera.pos = glm::vec3(0,0,0);
	//camera.lookAt = glm::vec3(0,0,1);
	camera.lookAt = glm::vec3(bear.modelMatrix[3][0], bear.modelMatrix[3][1], bear.modelMatrix[3][2]);
	camera.up = glm::vec3(0,1,0);
	camera.viewMatrix = glm::lookAt(camera.pos, camera.lookAt, camera.up);

	Frustum f;
	camera.GetFrustum(f);


	//// build noise cloud
	//cout << "building noise...\n";
	//// allocate verts
	//float *noiseVerts;
	//float *noiseColors;

	//float ***noiseValues;
	//noiseValues = new float**[100];
	//for (int i=0; i<100; i++)
	//{
	//	noiseValues[i] = new float*[100];
	//	for (int j=0; j<100; j++)
	//		noiseValues[i][j] = new float[100];
	//}
	//Perlin3DNoise perlin;
	////perlin.Generate3DArray(noiseValues, 100, 100, 100, 0.7, 0.2, 6, 0.5);
	////perlin.Generate3DArray(noiseValues, 100, 100, 100, 0.7, 0.5, 10, 0.5);
	//perlin.Generate3DArray(noiseValues, 100, 100, 100, 0.5, 0.2, 6, 0.5);
	//vector<Vertex> tempVerts;
	//for (int i=0; i<100; i++)
	//{
	//	for (int j=0; j<100; j++)
	//	{
	//		for (int k=0; k<100; k++)
	//		{
	//			if (noiseValues[i][j][k] > 0.5)
	//			{
	//				Vertex v;
	//				v.coordinates.x = i*0.1f;
	//				v.coordinates.y = j*0.1f;
	//				v.coordinates.z = k*0.1f;

	//				v.color.r = v.color.g = v.color.b = noiseValues[i][j][k];
	//				v.color.a = 1.0f;

	//				tempVerts.push_back(v);
	//			}
	//		}
	//	}
	//}
	//numNoisePoints = tempVerts.size();
	//noiseVerts = new float[numNoisePoints * 3];
	//noiseColors = new float[numNoisePoints * 4];
	//for (unsigned long i=0; i<numNoisePoints; i++)
	//{
	//	noiseVerts[i*3+0] = tempVerts[i].coordinates.x;
	//	noiseVerts[i*3+1] = tempVerts[i].coordinates.y;
	//	noiseVerts[i*3+2] = tempVerts[i].coordinates.z;

	//	noiseColors[i*4+0] = tempVerts[i].color.r;
	//	noiseColors[i*4+1] = tempVerts[i].color.g;
	//	noiseColors[i*4+2] = tempVerts[i].color.b;
	//	noiseColors[i*4+3] = tempVerts[i].color.a;
	//}	
	//
	//cout << "building vbos...\n";
	//glGenVertexArrays(1, &noiseVaoId);
	//glBindVertexArray(noiseVaoId);

	//glGenBuffers(1, &noiseVboId);
	//glBindBuffer(GL_ARRAY_BUFFER, noiseVboId);
	//glBufferData(GL_ARRAY_BUFFER, numNoisePoints * 3 * sizeof(float), noiseVerts, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(0);
	//
	//glGenBuffers(1, &noiseCboId);
	//glBindBuffer(GL_ARRAY_BUFFER, noiseCboId);
	//glBufferData(GL_ARRAY_BUFFER, numNoisePoints * 4 * sizeof(float), noiseColors, GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	//glBindVertexArray(0);
	//cout << "done!\n";

	cube.modelMatrix = glm::mat4();
	// end noise cloud
		
	
	FPSCounter::init();
	font.SetHDC(wglGetCurrentDC());
	font.BuildFont("Arial", 24);
}

int main(int argc, char **argv)
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Marching Cubes");
	
	init();

	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	//glutCloseFunc(CleanUp);
	glutMainLoop();

	return 1;
}