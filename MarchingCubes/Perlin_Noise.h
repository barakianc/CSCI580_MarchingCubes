#ifndef __PERLIN_NOISE_H__
#define __PERLIN_NOISE_H__

#define B 0x0100
#define BM 0x00ff
#define N 0x1000
#define NP 12
#define NM 0x0fff


class Perlin3DNoise
{
public:
	int p[B + B + 2];
	double g3[B + B + 2][3];
	double g2[B + B + 2][2];
	double g1[B + B + 2];
	int start;

	Perlin3DNoise();

	double PerlinNoise3D(double x,double y,double z,double alpha,double beta,int n);
	void Generate3DArray(float*** array,int xdim,int ydim,int zdim,double alpha,double beta,int n,double freq);

private:
	void init(void);
	double noise3(double vec[3]);
	void normalize3(double v[3]);
	void normalize2(double v[2]);
	double s_curve(double t);
	double lerp(double t,double a,double b);
	double at3(double rx,double ry, double rz,double* q);
	void setup(int i,int* b0,int* b1,double* r0,double* r1,double* vec);
};

#endif