#ifndef __MESHGENERATOR_HPP__
#define __MESHGENERATOR_HPP__

#include <iostream>
#include <math.h>

#include "MarchingCubes.hpp"
#include "Perlin_Noise.h"

using namespace std;


/*
cube corners have been named as
defined counter clockwise front face from bottom left, and then back face

				7			6	
								
		3			2
							
					
				4			5
						
		0			1

*/

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




//class MeshGenerator
//{
//private:
//	AllFans emptyAllFan;
//	McFan emptyFan;
//
//public:
//	int		cubeLength;
//	int		numCubes;
//	Cube	*cubes;
//	
//	int		lookUpFans[256];	// contains number of fans per cube
//	AllFans lookUpTable[256];	// contains list of cube cases
//	unsigned int numTriangles;
//
//	MeshGenerator()
//	{
//		MeshGenerator(60);
//	}
//
//	MeshGenerator(int cubeLength)
//	{
//		this->cubeLength = cubeLength;
//
//		//4,6,3
//		for(int i=0; i<4; i++)
//			for(int j=0; j<6; j++)
//				for(int k=0; k<3; k++)
//				{
//					emptyAllFan[i][j][k] = -1.0f;
//					emptyFan[j][k] = 0.0f;
//				}
//	}
//
//	void initCubes()
//	{
//		bool ***on;
//		float ***noise;
//
//		on = new bool**[cubeLength];
//		for(int i=0;i<cubeLength;i++)
//		{
//			on[i]=new bool*[cubeLength];
//			for(int j=0;j<cubeLength;j++)
//				on[i][j]=new bool[cubeLength];
//		}
//
//		noise=new float**[cubeLength];
//		for(int i=0;i<cubeLength;i++)
//		{
//			noise[i]=new float*[cubeLength];
//			for(int j=0;j<cubeLength;j++)
//				noise[i][j]=new float[cubeLength];
//		}
//			
//		Perlin3DNoise noises;
//		//noises.Generate3DArray(noise,cubeLength,cubeLength,cubeLength, .7 , .5, 10, 1.4);
//		noises.Generate3DArray(noise, cubeLength, cubeLength, cubeLength, 0.5, 0.2, 6, 0.5);
//
//		cubes = new Cube[cubeLength*cubeLength*cubeLength];
//		numCubes = (cubeLength-1)*(cubeLength-1)*(cubeLength-1);
//		onoff(on, noise, cubes);
//	}
//
//	void initFans()
//	{
//		bool testNodes[8];
//		int count;
//
//		for(int i=0; i<256; i++)
//		{
//			count = 0;	
//			testNodes[0] = (((unsigned char)i) & 0x01) == 1;
//			testNodes[1] = (((unsigned char)i) & 0x02) == 2;
//			testNodes[2] = (((unsigned char)i) & 0x04) == 4;
//			testNodes[3] = (((unsigned char)i) & 0x08) == 8;
//			testNodes[4] = (((unsigned char)i) & 0x10) == 16;
//			testNodes[5] = (((unsigned char)i) & 0x20) == 32;
//			testNodes[6] = (((unsigned char)i) & 0x40) == 64;
//			testNodes[7] = (((unsigned char)i) & 0x80) == 128;
//			memcpy(lookUpTable[i], emptyAllFan, sizeof(AllFans));
//
//			for(int j=0; j<8; j++)
//			{
//				if(testNodes[j])
//					count++;
//			}
//
//			if(count > 4)
//			{
//				for(int j=0; j<8; j++)
//					testNodes[j] = !testNodes[j];
//			}
//
//			getTriangleFan(testNodes, lookUpTable[i], lookUpFans[i]);
//		}
//	}
//
//private:
//
//	void onoff(bool*** on, float*** noise,Cube* cube)
//	{
//		bool tempBool[8];
//
//		for(int i=0;i<cubeLength;i++)
//			for(int j=0;j<cubeLength;j++)
//				for(int k=0;k<cubeLength;k++)
//					if(noise[i][j][k]>0.5)
//						on[i][j][k]=true;
//					else
//						on[i][j][k]=false;
//	
//		int cubeid=0;
//		for(int i=0;i<cubeLength-1;i++)
//		{
//			for(int j=0;j<cubeLength-1;j++)
//			{
//				for(int k=0;k<cubeLength-1;k++)
//				{
//					for(int y=0; y<8; y++)
//						tempBool[y]=false;
//
//					if(on[i][j][k]==1)
//						tempBool[0]=true;
//					if(on[i+1][j][k]==1)
//						tempBool[1]=true;
//					if(on[i+1][j+1][k]==1)
//						tempBool[2]=true;
//					if(on[i][j+1][k]==1)
//						tempBool[3]=true;
//					if(on[i][j][k+1]==1)
//						tempBool[4]=true;
//					if(on[i+1][j][k+1]==1)
//						tempBool[5]=true;
//					if(on[i+1][j+1][k+1]==1)
//						tempBool[6]=true;
//					if(on[i][j+1][k+1]==1)
//						tempBool[7]=true;
//
//					int counter=0;					
//					for(int y=0; y<8; y++)
//					{
//						if(tempBool[y]==true)
//							counter++;
//					}
//
//					// flip the normals
//					if(counter>4)
//					{
//						for(int y=0; y<8; y++)
//							tempBool[y]=!tempBool[y];
//					}
//
//					int boolsum = 0;
//					for(int y=0; y<8; y++)
//						if (tempBool[y])
//							boolsum += (int)pow(2.0, y);		
//
//					cube[cubeid].i=i;
//					cube[cubeid].j=j;
//					cube[cubeid].k=k;
//					cube[cubeid].vertformat=boolsum;
//
//					cubeid++;
//				}
//			}
//		}
//	}
//
//	int find_node_number(int x, int y, int z)
//	{
//		if(x==0)
//			if(y==0)
//				if(z==0)
//					return 0;
//				else
//					return 4;
//			else
//				if(z==0)
//					return 3;
//				else
//					return 7;
//		else
//			if(y==0)
//				if(z==0)
//					return 1;
//				else
//					return 5;
//			else
//				if(z==0)
//					return 2;
//				else
//					return 6;
//	}
//
//	void getMcNode(int node_number, McNode mcNode)
//	{
//		switch (node_number)
//		{
//			case 0: memcpy(mcNode, N0, sizeof(McNode)); break;
//			case 1: memcpy(mcNode, N1, sizeof(McNode)); break;		
//			case 2: memcpy(mcNode, N2, sizeof(McNode)); break;		
//			case 3: memcpy(mcNode, N3, sizeof(McNode)); break;		
//			case 4: memcpy(mcNode, N4, sizeof(McNode)); break;		
//			case 5: memcpy(mcNode, N5, sizeof(McNode)); break;		
//			case 6: memcpy(mcNode, N6, sizeof(McNode)); break;		
//			case 7: memcpy(mcNode, N7, sizeof(McNode)); break;
//		}
//	}
//
//	inline int oppositeOf(int number)
//	{
//		if(number == 1)
//			return 0;
//		else
//			return 1;
//	}
//
//	void getAdjacentNodes(int nodeNumber, AdjacentNodes adj)
//	{
//		McNode startNode;
//		getMcNode(nodeNumber, startNode);
//	
//		int adjacentNode0 = find_node_number(oppositeOf(startNode[0]), startNode[1], startNode[2]);
//		int adjacentNode1 = find_node_number(startNode[0], oppositeOf(startNode[1]), startNode[2]);
//		int adjacentNode2 = find_node_number(startNode[0], startNode[1], oppositeOf(startNode[2]));
//
//		adj[0] = adjacentNode0;
//		adj[1] = adjacentNode1;
//		adj[2] = adjacentNode2;
//	}
//
//	void getVertexBetween(int node1, int node2, McVertex mcVertex)
//	{
//		McNode n1, n2;
//		getMcNode(node1, n1);
//		getMcNode(node2, n2);
//
//		if(n1[0] != n2[0])
//		{
//			mcVertex[0]= .5;
//			mcVertex[1]= n1[1];
//			mcVertex[2]= n1[2];
//		}
//		if(n1[1] != n2[1])
//		{
//			mcVertex[0]= n1[0];
//			mcVertex[1]= .5;
//			mcVertex[2]= n1[2];
//		}
//		if(n1[2] != n2[2])
//		{
//			mcVertex[0]= n1[0];
//			mcVertex[1]= n1[1];
//			mcVertex[2]= .5;
//		}
//	}
//
//	void sortVerts(McFan fan_out, McFan fan_in, int numberOfVerts)
//	{
//		bool temp = false;
//		bool markedVerts[6];
//
//		for(int i=0; i<6; i++)
//			markedVerts[i] = false;
//
//		McVertex tempVert;
//		for(int i=0; i<numberOfVerts; i++)
//		{
//			if(i==0)
//			{
//				memcpy(fan_out[0], fan_in[0], sizeof(McVertex));
//				markedVerts[0] = true;
//			}
//			else
//			{
//				temp = false;
//				for(int j=0; j<numberOfVerts; j++)
//				{				
//					if((markedVerts[j] == false) && (temp == false))
//					{
//						for(int k=0; k<3; k++)
//						{					
//							if(((fan_out[i-1][k]==1) && (fan_in[j][k] == 1)) || ((fan_out[i-1][k]==0) && (fan_in[j][k] == 0)) )
//							{
//								temp = true;
//								markedVerts[j] = true;
//							}
//						}
//
//						if(temp == true)
//						{
//							memcpy(tempVert, fan_in[j], sizeof(McVertex));
//							memcpy(fan_out[i], fan_in[j], sizeof(McVertex));					
//						}
//					}
//				}
//			}
//		}
//	}
//
//	//returns the correct triangle fan assuming that only at most 4 bools on
//	void getTriangleFan(bool * nodesVal, AllFans allfans, int &numFans)
//	{
//		int vertexCounter= 0;
//		intQueue *first;
//		intQueue *last;
//		AdjacentNodes adj;
//		McVertex tempVert;
//		McFan tempFan;
//		bool marked[8];
//
//		numFans = 0;	
//		memcpy(tempFan, emptyFan, sizeof(McFan));
//
//		for(int i = 0; i<8; i++)
//			marked[i] = false;
//
//		for(int i=0; i<8; i++)
//		{
//			if((nodesVal[i] == true) && (marked[i] == false))
//			{
//				marked[i]=true;
//				first = new intQueue;
//				first->key = i;
//				first->next = NULL;
//				last = first;
//
//				while(first != NULL)
//				{
//					getAdjacentNodes(first->key, adj);
//					for(int j=0; j<3; j++)
//					{
//						if(nodesVal[adj[j]] == true)
//						{
//							if(marked[adj[j]] == false)
//							{
//								marked[adj[j]] = true;
//								intQueue * adjacentOn = new intQueue;
//								adjacentOn->key = adj[j];
//								adjacentOn->next = NULL;
//								last->next = adjacentOn;
//								last = last->next;
//							}
//						}
//						else
//						{
//							//add a triangle to the fan list
//							getVertexBetween(first->key, adj[j], tempVert);
//							memcpy(tempFan[vertexCounter], tempVert, sizeof(McVertex));
//							vertexCounter++;
//						}
//					}
//					first = first->next;
//				}
//				//return a sorted fan list
//				sortVerts(allfans[numFans], tempFan, vertexCounter);
//				vertexCounter=0;
//				numFans++;
//			}
//		}
//	}
//};


#endif