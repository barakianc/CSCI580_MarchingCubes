#ifndef __MARCHINGCUBES_HPP__
#define __MARCHINGCUBES_HPP__

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#include "Perlin_Noise.h"

using namespace std;

/* cube corners have been defined counter clockwise 
	front-face from bottom left, and then back face

			7			6	
								
	3			2
							
					
			4			5
						
	0			1
*/


typedef int McNode[3]; 
typedef double McVertex[3];
typedef int AdjacentNodes[3];
typedef McVertex McFan[6];
typedef McFan AllFans[4];
typedef int Faces[3];

const AllFans emptyAllFan = 
{
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0
};


const McNode N0 = { 0 , 0, 0};
const McNode N1 = { 1 , 0, 0};
const McNode N2 = { 1 , 1, 0};
const McNode N3 = { 0 , 1, 0};
const McNode N4 = { 0 , 0, 1};
const McNode N5 = { 1 , 0, 1};
const McNode N6 = { 1 , 1, 1};
const McNode N7 = { 0 , 1, 1};

const McFan emptyFan = 
{
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0
};

const McFan empty0Fan = 
{
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0
};

struct intQueue
{
	int key;
	intQueue * next;
};

struct vertNormal
{
	double n1, n2, n3;
};

class Cube
{
public:
	int i,j,k,vertformat;
	bool flipped;

	Cube()
	{
		i = j = k = vertformat = 0;
		flipped = false;
	}
};


class MarchingCubesGenerator
{
public:	
	Cube		***cubes;
	vertNormal	***allvertNormals;
	bool		***allvertBools;
	bool		tempBool[8];
	int			numCubes;	

	AllFans		lookUpTable[256];
	AllFans		lookUpNormTable[256];
	int			lookUpFans[256];

	unsigned long	numVerts;
	unsigned long	numTriangles;

	MarchingCubesGenerator()
	{
		MarchingCubesGenerator(50);
	}

	MarchingCubesGenerator(int numCubes)
	{
		this->numCubes = numCubes;
	}
	
	void getTriangleFan(bool * nodesVal, AllFans allfans, int &numFans, AllFans allFansNorm, bool flipped);
	void initFans();
	void initCubes();
	//void outToObj();

	void onoff(bool*** on, float*** noise);
	void initAllVertNormals();
	void checkOrAdd(McVertex vert, McVertex normal, bool flipped);
	void calcVertNormal(McVertex vert, Cube cube, McVertex normal, McVertex faceNormal);
	void crossProduct(McVertex vec1, McVertex vec2, McVertex normal, bool flipped, McVertex vert0, McVertex vert1, McVertex vert2, bool &order);
	void findCorrectNormal(McVertex vert, McVertex normal, Cube ***cube, AllFans lookUpTable[256], AllFans lookUpNormTable[256], int lookUpFans[256], int x, int y, int z);
	void getAdjacentNodes(int nodeNumber, AdjacentNodes adj);
	void getVertexBetween(int node1, int node2, McVertex mcVertex);
	void sortVerts(McFan fan_out, McFan fan_in, int numberOfVerts, McFan fanNorm, bool flipped, int onPoint);
	void getFaceNormals(McFan fan_out, McFan fanNorm, int numberOfVerts, bool flipped, int onPoint);
	void getVertNormals(McFan fan_out, McFan fanNorm, int numberOfVerts, bool flipped);
};

#endif