#include "estk.h"
#include <math.h>

// Matrices
esVec3f esMat3f_MulVec3f(const esMat3f *m, esVec3f v) {
	return (esVec3f) {
		v.x*m->mat[0] + v.y*m->mat[3] + v.z*m->mat[6],
		v.x*m->mat[1] + v.y*m->mat[4] + v.y*m->mat[7],
		v.x*m->mat[2] + v.y*m->mat[5] + v.z*m->mat[8],
	};
}

// Quaternions
esQuatf esQuatf_normalize(esQuatf q) {
	float mag = sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);

	if (mag > 0.0) {
		float inv = 1.0f / mag;
		q.x *= inv;
		q.y *= inv;
		q.z *= inv;
		q.w *= inv;
	}
	return q;
}

esQuatf esQuatf_mul(esQuatf q0, esQuatf q1) {
	return (esQuatf) {
		.w = q0.w*q1.w - q0.x*q1.x - q0.y*q1.y - q0.z*q1.z,
		.x = q0.w*q1.x + q0.x*q1.w + q0.y*q1.z - q0.z*q1.y,
		.y = q0.w*q1.y - q0.x*q1.z + q0.y*q1.w + q0.z*q1.x,
		.z = q0.w*q1.z + q0.x*q1.y - q0.y*q1.x + q0.z*q1.w,
	};
}

esQuatf esQuatf_local(esQuatf q, float angle) {
	float c = cosf(angle*0.5f);
	float s = sinf(angle*0.5f);
	return (esQuatf) {
		.x = q.x*s,
		.y = q.y*s,
		.z = q.z*s,
		.w = c,
	};
}

void esQuatf_matrix(esMat3f *dst, esQuatf q) {
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;
	float w2 = w*w;

	/*
	// Row 1
	dst->mat[0] = 1.0f - 2.0f*(y2 + z2);
	dst->mat[1] = 2.0f*(x*y - w*z);
	dst->mat[2] = 2.0f*(x*z + w*y);

	// Row 2
	dst->mat[3] = 2.0f*(x*y + w*z);
	dst->mat[4] = 1.0f - 2.0f*(x2 + z2);
	dst->mat[5] = 2.0f*(y*z + w*x);

	// Row 3
	dst->mat[6] = 2.0f*(x*z - w*y);
	dst->mat[7] = 2.0f*(y*z - w*x);
	dst->mat[8] = 1.0f - 2.0f*(x2 + y2);*/

	// Row 1
	dst->mat[0] = w2 + x2 - y2 - z2;
	dst->mat[1] = 2.0f * (x*y - w*z);
	dst->mat[2] = 2.0f * (x*z + w*y);

	// Row 2
	dst->mat[3] = 2.0f * (x*y + w*z);
	dst->mat[4] = w2 - x2 + y2 - z2;
	dst->mat[5] = 2.0f * (y*z + w*x);

	// Row 3
	dst->mat[6] = 2.0f * (x*z - w*y);
	dst->mat[7] = 2.0f * (y*z - w*x);
	dst->mat[8] = w2 - x2 - y2 + z2;
}

