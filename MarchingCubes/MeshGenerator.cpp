#include "MeshGenerator.hpp"


//int onoff2(bool*** on, float*** noise,Cubes* cube)
//{
//	for(int i=0;i<CubeLength;i++)
//	{
//		for(int j=0;j<CubeLength;j++)
//		{
//			for(int k=0;k<CubeLength;k++)
//			{
//				if(sqrt(((float)i) * ((float)i) + ((float)j)* ((float)j) + ((float)k) * ((float)k))<75)
//					on[i][j][k]=true;
//				else
//					on[i][j][k]=false;
//			}
//		}
//	}
//	
//	int cubeid=0;
//	for(int i=0;i<CubeLength-1;i++)
//	{
//		for(int j=0;j<CubeLength-1;j++)
//		{
//			for(int k=0;k<CubeLength-1;k++)
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
//				cube[cubeid].flipped = false;
//				for(int y=0; y<8; y++)
//				{
//					if(tempBool[y]==true)
//						counter++;
//				}
//				if(counter>4)
//				{
//					cube[cubeid].flipped = true;
//					for(int y=0; y<8; y++)
//						tempBool[y]=!tempBool[y];
//				}
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
//				cube[cubeid].i=i;
//				cube[cubeid].j=j;
//				cube[cubeid].k=k;
//				cube[cubeid].vertformat=boolsum;
//
//				boolsum=0;
//				cubeid++;
//				//checking complimentary cases 
//			}
//		}
//	}
//	return 0;
//}