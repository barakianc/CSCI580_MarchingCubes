#include "MarchingCubes.hpp"


//algorithm goes as follows

/*

For each unmarked node

if(adjacent node is off)
add vertex to triangle list
else
for each (on adjacent node)
	check each unmarked adjacent node
	if(...)
end

return vertex list triangle list
end

sort each triangle list for fan
*/



void MarchingCubesGenerator::onoff(bool*** on, float*** noise)
{
	for(int i=0;i<numCubes;i++)
	{
		for(int j=0;j<numCubes;j++)
		{
			for(int k=0;k<numCubes;k++)
			{
				if(noise[i][j][k]>0.5)
					on[i][j][k]=true;
				else
					on[i][j][k]=false;
			}
		}
	}
	
	for(int i=0;i<numCubes-1;i++)
	{
		for(int j=0;j<numCubes-1;j++)
		{
			for(int k=0;k<numCubes-1;k++)
			{
				cubes[i][j][k].vertformat = 0;

				for(int y=0; y<8; y++)
					tempBool[y]=false;

				if(on[i][j][k]==1)
					tempBool[0]=true;
				if(on[i+1][j][k]==1)
					tempBool[1]=true;
				if(on[i+1][j+1][k]==1)
					tempBool[2]=true;
				if(on[i][j+1][k]==1)
					tempBool[3]=true;
				if(on[i][j][k+1]==1)
					tempBool[4]=true;
				if(on[i+1][j][k+1]==1)
					tempBool[5]=true;
				if(on[i+1][j+1][k+1]==1)
					tempBool[6]=true;
				if(on[i][j+1][k+1]==1)
					tempBool[7]=true;

				int counter = 0;	
				int boolsum = 0;
				for(int y=0; y<8; y++)
				{
					if (tempBool[y])
					{
						counter++;
						boolsum += (int)pow(2.0, y);
					}
				}

				cubes[i][j][k].i=i;
				cubes[i][j][k].j=j;
				cubes[i][j][k].k=k;
				cubes[i][j][k].vertformat=boolsum;
			}
		}
	}
}

void MarchingCubesGenerator::initCubes()
{
	if (numCubes < 1)
		return;

	bool ***on = new bool**[numCubes];
	for(int i=0;i<numCubes;i++)
	{
		on[i]=new bool*[numCubes];
		for(int j=0;j<numCubes;j++)
			on[i][j]=new bool[numCubes];
	}
		
	float ***noise=new float**[numCubes];
	for(int i=0;i<numCubes;i++)
	{
		noise[i]=new float*[numCubes];
		for(int j=0;j<numCubes;j++)
			noise[i][j]=new float[numCubes];
	}
			
	Perlin3DNoise noises;
	noises.Generate3DArray(noise,numCubes,numCubes,numCubes, .7 , .5, 10, 1.4);	

	cubes=new Cube**[numCubes-1];
	for(int i=0; i<numCubes-1; i++)
	{
		cubes[i] = new Cube*[numCubes-1];
		for(int j=0; j<numCubes-1; j++)
			cubes[i][j] = new Cube[numCubes-1];
	}
	onoff(on, noise);

	initAllVertNormals();
}

void MarchingCubesGenerator::initFans()
{
	bool flipped;
	bool testNodes[8];
	int count;
	for(int i=0; i<256; i++)
	{
 		count = 0;	
		testNodes[0] = (((unsigned char)i) & 0x01) == 1;
		testNodes[1] = (((unsigned char)i) & 0x02) == 2;
		testNodes[2] = (((unsigned char)i) & 0x04) == 4;
		testNodes[3] = (((unsigned char)i) & 0x08) == 8;
		testNodes[4] = (((unsigned char)i) & 0x10) == 16;
		testNodes[5] = (((unsigned char)i) & 0x20) == 32;
		testNodes[6] = (((unsigned char)i) & 0x40) == 64;
		testNodes[7] = (((unsigned char)i) & 0x80) == 128;
		memcpy(lookUpTable[i], emptyAllFan, sizeof(AllFans));
		for(int j=0; j<8; j++)
		{
			if(testNodes[j])
				count++;
		}
		flipped = false;
		if(count > 4)
		{
			for(int j=0; j<8; j++)
			{
				testNodes[j] = !testNodes[j];
			}
			flipped = true;
		}
		getTriangleFan(testNodes, lookUpTable[i], lookUpFans[i], lookUpNormTable[i], flipped);
	}
}

//returns the correct triangle fan assuming that only at most 4 bools on
void MarchingCubesGenerator::getTriangleFan(bool * nodesVal, AllFans allfans, int &numFans, AllFans allFansNorm, bool flipped)
{
	int onPoint = 0;
	bool onPointset;
	numFans = 0;
	int vertexCounter= 0;
	intQueue * first;
	intQueue * last;
	AdjacentNodes adj;
	McVertex tempVert;
	McFan tempFan;
	memcpy(tempFan, emptyFan, sizeof(McFan));
	bool marked[8];
	for(int i = 0; i<8; i++)
	{
		marked[i] = false;
	}

	for(int i=0; i<8; i++)
	{
		onPointset=false;
		if((nodesVal[i] == true) && (marked[i] == false))
		{
			onPoint= i;
			marked[i]=true;
			first = new intQueue;
			first->key = i;
			first->next = NULL;
			last = first;
			while(first != NULL)
			{
				getAdjacentNodes(first->key, adj);
				for(int j=0; j<3; j++)
				{
					if(nodesVal[adj[j]] == true)
					{
						if(marked[adj[j]] == false)
						{
							marked[adj[j]] = true;
							intQueue * adjacentOn = new intQueue;
							adjacentOn->key = adj[j];
							adjacentOn->next = NULL;
							last->next = adjacentOn;
							last = last->next;
						}
					}
					else
					{
						//add a triangle to the fan list
						getVertexBetween(first->key, adj[j], tempVert);
						memcpy(tempFan[vertexCounter], tempVert, sizeof(McVertex));
						vertexCounter++;
					}
				}
				first = first->next;
			}
			//return a sorted fan list
			sortVerts(allfans[numFans], tempFan, vertexCounter, allFansNorm[numFans], flipped, onPoint);
			
			vertexCounter=0;
			numFans++;
		}
	}
}


int find_node_number(int x, int y, int z)
{
	if(x==0)
	{
		if(y==0)
		{
			if(z==0)
			{
				return 0;
			}
			else
			{
				return 4;
			}
		}
		else
		{
			if(z==0)
			{
				return 3;
			}
			else
			{
				return 7;
			}
		}
	}
	else
	{
		if(y==0)
		{
			if(z==0)
			{
				return 1;
			}
			else
			{
				return 5;
			}
		}
		else
		{
			if(z==0)
			{
				return 2;
			}
			else
			{
				return 6;
			}
		}
	}
}


void getMcNode(int node_number, McNode mcNode)
{
	switch (node_number)
	{
		case 0:
			memcpy(mcNode, N0, sizeof(McNode));
		break;

		case 1:
			memcpy(mcNode, N1, sizeof(McNode));
		break;
		
		case 2:
			memcpy(mcNode, N2, sizeof(McNode));
		break;
		
		case 3:
			memcpy(mcNode, N3, sizeof(McNode));
		break;
		
		case 4:
			memcpy(mcNode, N4, sizeof(McNode));
		break;
		
		case 5:
			memcpy(mcNode, N5, sizeof(McNode));
		break;
		
		case 6:
			memcpy(mcNode, N6, sizeof(McNode));
		break;
		
		case 7:
			memcpy(mcNode, N7, sizeof(McNode));
		break;


	}
}

int oppositeOf(int number)
{
	if(number == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void MarchingCubesGenerator::getAdjacentNodes(int nodeNumber, AdjacentNodes adj)
{
	McNode startNode;
	getMcNode(nodeNumber, startNode);
	
	int adjacentNode0 = find_node_number(oppositeOf(startNode[0]), startNode[1], startNode[2]);
	int adjacentNode1 = find_node_number(startNode[0], oppositeOf(startNode[1]), startNode[2]);
	int adjacentNode2 = find_node_number(startNode[0], startNode[1], oppositeOf(startNode[2]));

	adj[0] = adjacentNode0;
	adj[1] = adjacentNode1;
	adj[2] = adjacentNode2;

}

void MarchingCubesGenerator::getVertexBetween(int node1, int node2, McVertex mcVertex)
{
	McNode n1, n2;
	getMcNode(node1, n1);
	getMcNode(node2, n2);

	if(n1[0] != n2[0])
	{
		mcVertex[0]= .5;
		mcVertex[1]= n1[1];
		mcVertex[2]= n1[2];
	}
	if(n1[1] != n2[1])
	{
		mcVertex[0]= n1[0];
		mcVertex[1]= .5;
		mcVertex[2]= n1[2];
	}
	if(n1[2] != n2[2])
	{
		mcVertex[0]= n1[0];
		mcVertex[1]= n1[1];
		mcVertex[2]= .5;
	}
}

void MarchingCubesGenerator::getFaceNormals(McFan fan_out, McFan fanNorm, int numberOfVerts, bool flipped, int onPoint)
{
	McNode point;
	McVertex vec3;
	McVertex vec4;
	McFan goodFan;
	McVertex normal;
	McVertex tempNormal;
	McVertex opposite;
	double magnitude;
	
	memcpy(goodFan, emptyFan, sizeof(McFan));

	for(int i=0; i<numberOfVerts; i++)
	{
		if(i>1)
		{
			//get vectors
			vec3[0] = fan_out[i-1][0] - fan_out[0][0];
			vec3[1] = fan_out[i-1][1] - fan_out[0][1];
			vec3[2] = fan_out[i-1][2] - fan_out[0][2];
			
			vec4[0] = fan_out[i][0] - fan_out[0][0];
			vec4[1] = fan_out[i][1] - fan_out[0][1];
			vec4[2] = fan_out[i][2] - fan_out[0][2];
		
			//create a normal from the vectors via cross product
			normal[0] = vec3[1] * vec4[2]  - vec3[2] * vec4[1];
			normal[1] = vec3[2] * vec4[0]  - vec3[0] * vec4[2];
			normal[2] = vec3[0] * vec4[1]  - vec3[1] * vec4[0];

			//make sure points correctly
			getMcNode(onPoint, point);
			opposite[0] = ((double)point[0]) - fan_out[0][0];
			opposite[1] = ((double)point[1]) - fan_out[0][1];
			opposite[2] = ((double)point[2]) - fan_out[0][2];
	
			
			magnitude = sqrt(opposite[0] * opposite[0] + opposite[1] * opposite[1] + opposite[2] * opposite[2]);
			opposite[0] = opposite[0]/magnitude;
			opposite[1] = opposite[1]/magnitude;
			opposite[2] = opposite[2]/magnitude;

			memcpy(tempNormal, normal, sizeof(McVertex));
			magnitude = sqrt(tempNormal[0] * tempNormal[0] + tempNormal[1] * tempNormal[1] + tempNormal[2] * tempNormal[2]);
			tempNormal[0] = tempNormal[0] / magnitude;
			tempNormal[1] = tempNormal[1] / magnitude;
			tempNormal[2] = tempNormal[2] / magnitude;

			if(tempNormal[0]*opposite[0] + tempNormal[1]*opposite[1] + tempNormal[2]*opposite[2] > 0)
			{
				memcpy(goodFan[0], fan_out[0], sizeof(McVertex));
				for(int j=numberOfVerts-1; j>=1; j--)
				{
					memcpy(goodFan[j], fan_out[numberOfVerts-j], sizeof(McVertex));
				}
				normal[0] = -normal[0];
				normal[1] = -normal[1];
				normal[2] = -normal[2];
				memcpy(fan_out, goodFan, sizeof(McFan));


			}
			
			
			if( flipped)
			{
				memcpy(goodFan[0], fan_out[0], sizeof(McVertex));
				for(int j=numberOfVerts-1; j>=1; j--)
				{
					memcpy(goodFan[j], fan_out[numberOfVerts-j], sizeof(McVertex));
				}
				memcpy(fan_out, goodFan, sizeof(McFan));
				normal[0] = -normal[0];
				normal[1] = -normal[1];
				normal[2] = -normal[2];
			}
		

	/*
			vec3[0] = fan_out[i-1][0] - fan_out[0][0];
			vec3[1] = fan_out[i-1][1] - fan_out[0][1];
			vec3[2] = fan_out[i-1][2] - fan_out[0][2];
			
			vec4[0] = fan_out[i][0] - fan_out[0][0];
			vec4[1] = fan_out[i][1] - fan_out[0][1];
			vec4[2] = fan_out[i][2] - fan_out[0][2];
		
			//create a normal from the vectors via cross product
			normal[0] = vec3[1] * vec4[2]  - vec3[2] * vec4[1];
			normal[1] = vec3[2] * vec4[0]  - vec3[0] * vec4[2];
			normal[2] = vec3[0] * vec4[1]  - vec3[1] * vec4[0];


			*/
			//bug sum  normals
			fanNorm[0][0] -= normal[0];
			fanNorm[0][1] -= normal[1];
			fanNorm[0][2] -= normal[2];

			fanNorm[i-1][0] -= normal[0];
			fanNorm[i-1][1] -= normal[1];
			fanNorm[i-1][2] -= normal[2];

			fanNorm[i][0] -= normal[0];
			fanNorm[i][1] -= normal[1];
			fanNorm[i][2] -= normal[2];
		}

	}
}

void MarchingCubesGenerator::getVertNormals(McFan fan_out, McFan fanNorm, int numberOfVerts, bool flipped)
{
	McVertex normal;
	McVertex vec3;
	McVertex vec4;
	memcpy(fanNorm, empty0Fan, sizeof(McFan));

	
	for(int i=0; i<numberOfVerts; i++)
	{
		if(i>1)
		{
			//get vectors
			vec3[0] = fan_out[i-1][0] - fan_out[0][0];
			vec3[1] = fan_out[i-1][1] - fan_out[0][1];
			vec3[2] = fan_out[i-1][2] - fan_out[0][2];
			
			vec4[0] = fan_out[i][0] - fan_out[0][0];
			vec4[1] = fan_out[i][1] - fan_out[0][1];
			vec4[2] = fan_out[i][2] - fan_out[0][2];
		
			//create a normal from the vectors via cross product
			normal[0] = vec3[1] * vec4[2]  - vec3[2] * vec4[1];
			normal[1] = vec3[2] * vec4[0]  - vec3[0] * vec4[2];
			normal[2] = vec3[0] * vec4[1]  - vec3[1] * vec4[0];


			fanNorm[0][0] -= normal[0];
			fanNorm[0][1] -= normal[1];
			fanNorm[0][2] -= normal[2];

			fanNorm[i-1][0] -= normal[0];
			fanNorm[i-1][1] -= normal[1];
			fanNorm[i-1][2] -= normal[2];

			fanNorm[i][0] -= normal[0];
			fanNorm[i][1] -= normal[1];
			fanNorm[i][2] -= normal[2];
		}
	}
}

void MarchingCubesGenerator::sortVerts(McFan fan_out, McFan fan_in, int numberOfVerts, McFan fanNorm, bool flipped, int onPoint)
{
	bool temp = false;
	bool markedVerts[6];
	for(int i=0; i<6; i++)
	{
		markedVerts[i] = false;
	}
	McVertex tempVert;
	for(int i=0; i<numberOfVerts; i++)
	{
		if(i==0)
		{
			memcpy(fan_out[0], fan_in[0], sizeof(McVertex));
			markedVerts[0] = true;
		}
		else
		{
			temp = false;
			for(int j=0; j<numberOfVerts; j++)
			{
				
				if((markedVerts[j] == false) && (temp == false))
				{
					for(int k=0; k<3; k++)
					{
					
						if(((fan_out[i-1][k]==1) && (fan_in[j][k] == 1)) || ((fan_out[i-1][k]==0) && (fan_in[j][k] == 0)) )
						{
							temp = true;
							markedVerts[j] = true;
						}
					}

					if(temp == true)
					{
						memcpy(tempVert, fan_in[j], sizeof(McVertex));
						memcpy(fan_out[i], fan_in[j], sizeof(McVertex));
						
					
					}
				}
			}
			
		}
	}
	
	getFaceNormals(fan_out, fanNorm, numberOfVerts, flipped, onPoint);
	getVertNormals(fan_out, fanNorm, numberOfVerts, flipped);
}

void MarchingCubesGenerator::initAllVertNormals()
{
	allvertNormals = new vertNormal**[numCubes];
	allvertBools = new bool**[numCubes];
	for(int i=0; i<numCubes; i++)
	{
		allvertNormals[i] = new vertNormal*[numCubes];
		allvertBools[i] = new bool*[numCubes];
		for(int j=0; j<numCubes; j++)
		{
			allvertNormals[i][j] =  new vertNormal[numCubes];
			allvertBools[i][j] = new bool[numCubes];
			for(int k=0; k<numCubes; k++)
			{
				//allvertNormals[i][j] =  new vertNormal[cubeSize];
				allvertBools[i][j][k] = false;
			}
		}
	}
}

void MarchingCubesGenerator::checkOrAdd(McVertex vert, McVertex normal, bool flipped)
{
	int i,j,k;
	i = 2 * (int)vert[0];
	j = 2 * (int)vert[1];
	k = 2 * (int)vert[2];

	double x,y,z;
	x = vert[0] - (int)(vert[0]);
	y = vert[1] - (int)(vert[1]);
	z = vert[2] - (int)(vert[2]);

	if(allvertBools[i][j][k] == false)
	{
		allvertBools[i][j][k] = true;
		normal[0] = vert[0] - .5;
		normal[1] = vert[1] - .5;
		normal[2] = vert[2] - .5;
	}
	else
	{
		normal[0] = allvertNormals[i][j][k].n1;
		normal[1] = allvertNormals[i][j][k].n2;
		normal[2] = allvertNormals[i][j][k].n3;
	}
}

void MarchingCubesGenerator::calcVertNormal(McVertex vert, Cube cube, McVertex normal, McVertex faceNormal)
{
	//normal[0] = -vert[0]-cube.i + cubeLength/2;
	//normal[1] = -vert[1]-cube.j + cubeLength/2;
	//normal[2] = -vert[2]-cube.k + cubeLength/2;

	//double magnitude;
	//magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	//normal[0] = normal[0] / magnitude;
	//normal[1] = normal[1] / magnitude;
	//normal[2] = normal[2] / magnitude;

	normal[0] = vert[0]-.5;
	normal[1] = vert[1]-.5;
	normal[2] = vert[2]-.5;

	if(cube.flipped)
	{
		//normal[0] = -normal[0];
		//normal[1] = -normal[1];
		//normal[2] = -normal[2];
	}

	double dotproduct;
	dotproduct = faceNormal[0] * normal[0] + faceNormal[1] * normal[1] + faceNormal[1] * normal[1] ;
	if(dotproduct == 0)
	{
		memcpy(normal, faceNormal, sizeof(McVertex));
	}
	else
	{
		double magnitude;
		magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
		normal[0] = normal[0] / magnitude;
		normal[1] = normal[1] / magnitude;
		normal[2] = normal[2] / magnitude;

		normal[0] += faceNormal[0];
		normal[1] += faceNormal[1];
		normal[2] += faceNormal[2];

		magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
		normal[0] = normal[0] / magnitude;
		normal[1] = normal[1] / magnitude;
		normal[2] = normal[2] / magnitude;

	}
}

void MarchingCubesGenerator::crossProduct(McVertex vec1, McVertex vec2, McVertex normal, bool flipped, McVertex vert0, McVertex vert1, McVertex vert2, bool &order)
{
	order = false;
									//vert normals
	vec1[0] = vert1[0] - vert0[0];
	vec1[1] = vert1[1] - vert0[1];
	vec1[2] = vert1[2] - vert0[2];
	vec2[0] = vert2[0] - vert0[0];
	vec2[1] = vert2[1] - vert0[1];
	vec2[2] = vert2[2] - vert0[2];

	McVertex pretendNormal;
	pretendNormal[0] = vert0[0] - .50;
	pretendNormal[1] = vert0[1] - .50;
	pretendNormal[2] = vert0[2] - .50;


	double magnitude;
	
	normal[0] = vec1[1] * vec2[2]  - vec1[2] * vec2[1];
	normal[1] = vec1[2] * vec2[0]  - vec1[0] * vec2[2];
	normal[2] = vec1[0] * vec2[1]  - vec1[1] * vec2[0];
	magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	normal[0] = normal[0] / magnitude;
	normal[1] = normal[1] / magnitude;
	normal[2] = normal[2] / magnitude;

	magnitude = sqrt(pretendNormal[0] * pretendNormal[0] + pretendNormal[1] * pretendNormal[1] + pretendNormal[2] * pretendNormal[2]);
	pretendNormal[0] = pretendNormal[0] / magnitude;
	pretendNormal[1] = pretendNormal[1] / magnitude;
	pretendNormal[2] = pretendNormal[2] / magnitude;
	
	double dotproduct = normal[0] * pretendNormal[0] + normal[1] * pretendNormal[1] + normal[2] * pretendNormal[2];

	if(dotproduct <= 0)
	{
		order= true;
		for(int i=0; i<3; i++)
		{
			normal[i] *= -1;
		}

	}

			
	if(!flipped)
	{
		order= !order;
		for(int i=0; i<3; i++)
		{
			//normal[i] *= -1;
		}
	}

}

void MarchingCubesGenerator::findCorrectNormal(McVertex vert, McVertex normal, Cube ***cube, AllFans lookUpTable[256], AllFans lookUpNormTable[256], int lookUpFans[256], int x, int y, int z)
{
	normal[0] = 0;
	normal[1] = 0;
	normal[2] = 0;
	
	
	if(x>0 && x<numCubes-2)
	{
		if(y>1 && y<numCubes-2)
		{
			if(z>1 && z<numCubes-2)
			{
				for(int i=-1; i<2; i++)
				{
					for(int j=-1; j<2; j++)
					{
						for(int k=-1; k<2; k++)
						{
							for(int h=0; h<lookUpFans[cube[x][y][z].vertformat]; h++)
							{
								for(int l=0; l<6; l++)
								{
									if(lookUpTable[cube[x+i][y+j][z+k].vertformat][h][l][0] != -1)
									{
										if((vert[0] == lookUpTable[cube[x+i][y+j][z+k].vertformat][h][l][0]+i) && 
											(vert[1] == lookUpTable[cube[x+i][y+j][z+k].vertformat][h][l][1]+j) && 
											(vert[2] == lookUpTable[cube[x+i][y+j][z+k].vertformat][h][l][2]+k))
										{
											normal[0]+=lookUpNormTable[cube[x+i][y+j][z+k].vertformat][h][l][0];
											normal[1]+=lookUpNormTable[cube[x+i][y+j][z+k].vertformat][h][l][1];
											normal[2]+=lookUpNormTable[cube[x+i][y+j][z+k].vertformat][h][l][2];
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	double magnitude;
	magnitude = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	normal[0] = normal[0] / magnitude;
	normal[1] = normal[1] / magnitude;
	normal[2] = normal[2] / magnitude;
							
}


//

// USED ONLY FOR DEBUGGING
//void onoff2(bool*** on, float*** noise,Cubes*** cube)
//{
//	for(int i=0;i<cubeLength;i++)
//	{
//		for(int j=0;j<cubeLength;j++)
//		{
//			for(int k=0;k<cubeLength;k++)
//			{
//				if(sqrt((double(i-10) * double(i-10))+(double(j-10) * double(j-10))+(double(k-10) * double(k-10)))<5)
//				//if((i==5) && (j==5 || j==6) && (k>5 && k<10))
//					on[i][j][k]=true;
//				else
//					on[i][j][k]=false;
//			}
//		}
//	}
//	
//	int cubeid=0;
//	for(int i=0;i<cubeLength-1;i++)
//	{
//		for(int j=0;j<cubeLength-1;j++)
//		{
//			for(int k=0;k<cubeLength-1;k++)
//			{
//				for(int y=0; y<8; y++)
//					tempBool[y]=false;
//
//				if(on[i][j][k]==1)
//					tempBool[0]=true;
//				if(on[i+1][j][k]==1)
//					tempBool[1]=true;
//				if(on[i+1][j+1][k]==1)
//					tempBool[2]=true;
//				if(on[i][j+1][k]==1)
//					tempBool[3]=true;
//				if(on[i][j][k+1]==1)
//					tempBool[4]=true;
//				if(on[i+1][j][k+1]==1)
//					tempBool[5]=true;
//				if(on[i+1][j+1][k+1]==1)
//					tempBool[6]=true;
//				if(on[i][j+1][k+1]==1)
//					tempBool[7]=true;
//
//				int counter=0;					
//				for(int y=0; y<8; y++)
//				{
//					if(tempBool[y]==true)
//					counter++;
//				}
//				cube[i][j][k].flipped=false;
//
//				int boolsum = 0;
//				if(tempBool[0])
//					boolsum++;
//				if(tempBool[1])
//					boolsum+=2;
//				if(tempBool[2])
//					boolsum+=4;
//				if(tempBool[3])
//					boolsum+=8;
//				if(tempBool[4])
//					boolsum+=16;
//				if(tempBool[5])
//					boolsum+=32;
//				if(tempBool[6])
//					boolsum+=64;
//				if(tempBool[7])
//					boolsum+=128;
//					
//
//				cube[i][j][k].i=i;
//				cube[i][j][k].j=j;
//				cube[i][j][k].k=k;
//				cube[i][j][k].vertformat=boolsum;
//				boolsum=0;
//			}
//		}
//	}
//}