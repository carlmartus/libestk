#include "estk.h"
#include <math.h>

#define P0 (0.0f)
#define P1 (1.0f)

void identity_matrix(float *mat) {
	mat[ 0]=P1; mat[ 4]=P0; mat[ 8]=P0; mat[12]=P0;
	mat[ 1]=P0; mat[ 5]=P1; mat[ 9]=P0; mat[13]=P0;
	mat[ 2]=P0; mat[ 6]=P0; mat[10]=P1; mat[14]=P0;
	mat[ 3]=P0; mat[ 7]=P0; mat[11]=P0; mat[15]=P1;
}

void esProj_identity(esMat4f *mat) {
	float *m = mat->mat;
	identity_matrix(m);
}

void esProj_ortho(esMat4f *mat, float x0, float y0, float x1, float y1) {
	float *m = mat->mat;

	m[ 1]=P0; m[ 2]=P0;
	m[ 4]=P0; m[ 6]=P0;
	m[ 8]=P0; m[ 9]=P0;
	m[12]=P0; m[13]=P0; m[14] = P0;

	m[ 0] = 2.0f / (x1-x0);
	m[ 5] = 2.0f / (y1-y0);
	m[10] = -1.0f;
	m[15] = P1;

	m[12] = -(x1+x0)/(x1-x0);
	m[13] = -(y1+y0)/(y1-y0);
	m[14] = 0.0f;
}

static void perspective_matrix(float *mat,
		float fov, float screenratio, float near, float far) {
	float size = near * tanf(fov * 0.5); 
	float left = -size;
	float right = size;
	float bottom = -size / screenratio;
	float top = size / screenratio;

	mat[0] = 2 * near / (right - left);
	mat[1] = 0.0;
	mat[2] = 0.0;
	mat[3] = 0.0;

	mat[4] = 0.0;
	mat[5] = 2 * near / (top - bottom);
	mat[6] = 0.0;
	mat[7] = 0.0;

	mat[8] = (right + left) / (right - left);
	mat[9] = (top + bottom) / (top - bottom);
	mat[10] = -(far + near) / (far - near);
	mat[11] = -1;

	mat[12] = 0.0;
	mat[13] = 0.0;
	mat[14] = -(2 * far * near) / (far - near);
	mat[15] = 0.0;
}


static esVec3f cross(esVec3f a, esVec3f b) {
	return (esVec3f) {
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x,
	};
}

static void normalize(esVec3f *v) {
    float r;

    r = sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
    if (r == 0.0f) return;

	r = 1.0f / r;
    v->x *= r;
    v->y *= r;
    v->z *= r;
}

/*
static float
dot(esVec3f a, esVec3f b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}*/

void mul_matrix(float *res, float *a, float *b) {
	int x, y, i=0;
	for (y=0; y<4; y++) {
		for (x=0; x<4; x++) {

			int r = y<<2;
			res[i] =
				a[r+ 0]*b[x+ 0] +
				a[r+ 1]*b[x+ 4] +
				a[r+ 2]*b[x+ 8] +
				a[r+ 3]*b[x+12];

			i++;
		}
	}
}

void lookat_matrix(float *mat, esVec3f eye, esVec3f at, esVec3f up) {
	esVec3f forw = {
		at.x - eye.x,
		at.y - eye.y,
		at.z - eye.z,
	};

	normalize(&forw);
	esVec3f side = cross(up, forw);
	normalize(&side);

	up = cross(forw, side);

	float m0[16];
	identity_matrix(m0);

	m0[ 0] = side.x;
	m0[ 4] = side.y;
	m0[ 8] = side.z;

	m0[ 1] = up.x;
	m0[ 5] = up.y;
	m0[ 9] = up.z;

	m0[ 2] = -forw.x;
	m0[ 6] = -forw.y;
	m0[10] = -forw.z;

	float m1[16];
	identity_matrix(m1);

	m1[12] = -eye.x;
	m1[13] = -eye.y;
	m1[14] = -eye.z;

	mul_matrix(mat, m1, m0);
}

void esProj_perspective(
		esMat4f *mat, float fov, float screenratio, float near, float far,
		esVec3f eye, esVec3f at, esVec3f up) {
	float persp[16];
	perspective_matrix(persp, fov, screenratio, near, far);

	float look[16];
	lookat_matrix(look, eye, at, up);

	mul_matrix(mat->mat, look, persp);
}

