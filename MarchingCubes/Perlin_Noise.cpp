#include "Perlin_Noise.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;


Perlin3DNoise::Perlin3DNoise()
{
	start = 1;
}

double Perlin3DNoise::PerlinNoise3D(double x,double y,double z,double alpha,double beta,int n)
{
	int i;
	double val,sum = 0;
	double p[3],scale = 1;

	p[0] = x;
	p[1] = y;
	p[2] = z;
	for (i=0;i<n;i++) 
	{
		val = noise3(p);
		sum += val / scale;
		scale *= alpha;
		p[0] *= beta;
		p[1] *= beta;
		p[2] *= beta;
	}

	return sum;
}

void Perlin3DNoise::init(void)
{
	int i, j, k;

	//srand((unsigned)time(NULL));
	srand(0);

	for (i = 0 ; i < B ; i++) 
	{
		p[i] = i;
		g1[i] = (double)((rand() % (B + B)) - B) / B;

		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (double)((rand() % (B + B)) - B) / B;
		normalize2(g2[i]);

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (double)((rand() % (B + B)) - B) / B;
		normalize3(g3[i]);
	}

	while (--i)
	{
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++)
	{
		p[B + i] = p[i];
		g1[B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}
}

double Perlin3DNoise::noise3(double vec[3])
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	int i, j;

	if (start)
	{
		start = 0;
		init();
	}

	setup(0, &bx0,&bx1, &rx0,&rx1,vec);
	setup(1, &by0,&by1, &ry0,&ry1,vec);
	setup(2, &bz0,&bz1, &rz0,&rz1,vec);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0,q);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0,q);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0,q);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0,q);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1,q);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1,q);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1,q);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1,q);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

void Perlin3DNoise::normalize3(double v[3])
{
	double s;

	s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}

void Perlin3DNoise::normalize2(double v[2])
{
	double s;

	s = sqrt(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
}

double Perlin3DNoise::s_curve(double t)
{
	return (t * t * (3. - 2. * t));
}

double Perlin3DNoise::lerp(double t,double a,double b)
{
	return ( a + t * (b - a) );
}

double Perlin3DNoise::at3(double rx,double ry, double rz,double* q)
{
	return ( rx * q[0] + ry * q[1] + rz * q[2] );
}

void Perlin3DNoise::setup(int i,int* b0,int* b1,double* r0,double* r1,double* vec)
{
	//srand((unsigned)time(NULL));	
	srand(0);
	double t = vec[i] + N;
	*b0 = ((int)t) & BM;
    *b1 = (*b0+1) & BM;
    *r0 = t +(((double)rand()/(double)RAND_MAX)/2) - (int)t;
    *r1 = *r0 - 1;
}

void Perlin3DNoise::Generate3DArray(float*** array3D,int xdim,int ydim,int zdim,double alpha,double beta,int n,double freq)
{
	for(int i = 0; i < xdim; i ++)
	{
		for(int j = 0; j < ydim; j++)
		{
			for(int k = 0; k < zdim; k++)
			{
				//srand((unsigned)time(NULL));
				double r1 = ((rand() % 32)/128);
				double r2 = ((rand() % 32)/128);
				double r3 = ((rand() % 32)/128);
				array3D[i][j][k] = ((1.0f + (float)(freq*PerlinNoise3D(i+r1,j+r2,k+r3,alpha,beta,n)))/2.0f);
			}
		}
	}
}