#include<iostream>
#include"Perlin_Noise.h"
using namespace std;
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
typedef struct 
{
	int i,j,k,vertformat;
}cubes;


	/*
	cube corners have been named as

									7			6	
								
							3			2
							
					
									4			5
						
							0			1

	*/
	int onoff(bool*** on, float*** noise,cubes* cube)
	{
		int boolsum=0;

		for(int i=0;i<100;i++)
		{
			for(int j=0;j<100;j++)
			{
				for(int k=0;k<100;k++)
				{
					if(noise[i][j][k]>0.5)
						noise[i][j][k]=1;
					else
						noise[i][j][k]=0;
					on[i][j][k]=noise[i][j][k];
				}
			}
		}
	
		int cubeid=0;
		for(int i=0;i<99;i++)
			for(int j=0;j<99;j++)
				for(int k=0;k<99;k++)
				{
					int counton=0;
					if(noise[i][j][k]==1)
						counton++;
					if(noise[i][j][k+1]==1)
						counton++;
					if(noise[i][j+1][k]==1)
						counton++;
					if(noise[i][j+1][k+1]==1)
						counton++;
					if(noise[i+1][j][k]==1)
						counton++;
					if(noise[i+1][j][k+1]==1)
						counton++;
					if(noise[i+1][j+1][k]==1)
						counton++;
					if(noise[i+1][j+1][k+1]==1)
						counton++;

					cout<<"\n on-off values for cube at "<<i<<","<<j<<","<<k<<" are: ";
					cout<<on[i][j][k]<<on[i][j][k+1]<<on[i][j+1][k]<<on[i][j+1][k+1]<<on[i+1][j][k]<<on[i+1][j][k+1]<<on[i+1][j+1][k]<<on[i+1][j+1][k+1];


				
					if(counton>4)
					{
					cout<<"\nComplemented values for cube for cube at "<<i<<","<<j<<","<<k<<" are: ";

					if(on[i][j][k]==1)
						on[i][j][k]=0;
					else
						on[i][j][k]=1;

					if(on[i][j][k+1]==1)
						on[i][j][k+1]=0;
					else
						on[i][j][k+1]=1;
				
					if(on[i][j+1][k]==1)
						on[i][j+1][k]=0;
					else
						on[i][j+1][k]=1;

					if(on[i][j+1][k+1]==1)
						on[i][j+1][k+1]=0;
					else
						on[i][j+1][k+1]=1;

					if(on[i+1][j][k]==1)
						on[i+1][j][k]=0;
					else
						on[i+1][j][k]=1;

					if(on[i+1][j][k+1]==1)
						on[i+1][j][k+1]=0;
					else
						on[i+1][j][k+1]=1;

					if(on[i+1][j+1][k]==1)
						on[i+1][j+1][k]=0;
					else
						on[i+1][j+1][k]=1;

					if(on[i+1][j+1][k+1]==1)
						on[i+1][j+1][k+1]=0;
					else
						on[i+1][j+1][k+1]=1;

					cout<<on[i][j][k]<<on[i][j][k+1]<<on[i][j+1][k]<<on[i][j+1][k+1]<<on[i+1][j][k]<<on[i+1][j][k+1]<<on[i+1][j+1][k]<<on[i+1][j+1][k+1];
					}
			if(on[i][j][k]==1)
				boolsum++;
			if(on[i+1][j][k]==1)
				boolsum+=2;
			if(on[i+1][j+1][k]==1)
				boolsum+=4;
			if(on[i][j+1][k]==1)
				boolsum+=8;
			if(on[i][j][k+1]==1)
				boolsum+=16;
			if(on[i+1][j][k+1]==1)
				boolsum+=32;
			if(on[i+1][j+1][k+1]==1)
				boolsum+=64;
			if(on[i][j+1][k+1]==1)
				boolsum+=128;

				cube[cubeid].i=i;
				cube[cubeid].j=j;
				cube[cubeid].k=k;
				cube[cubeid].vertformat=boolsum;
				cout<<"\nvertfrmat"<<cube[cubeid].vertformat;
				boolsum=0;
				cubeid++;
		//checking complimentary cases 
	

			}
		return 0;
	}

	/*int main()
	{
		bool ***on;
		on = new bool**[100];
		for(int i=0;i<100;i++)
			{
				on[i]=new bool*[100];
				for(int j=0;j<100;j++)
					on[i][j]=new bool[100];
		    }

	 float*** noise;
	    noise=new float**[100];
		for(int i=0;i<100;i++)
			{
				noise[i]=new float*[100];
				for(int j=0;j<100;j++)
					noise[i][j]=new float[100];
		    }
			
		Perlin3DNoise noises;
	noises.Generate3DArray(noise,100,100,100,2,2,3,1.5);

		cubes *cube;
		cube=new cubes[1000000];
		int status=onoff(on,noise,cube);
		getchar();
	}*/
	

