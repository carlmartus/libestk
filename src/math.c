#include "estk.h"
#include <math.h>

esQuatf esQuatf_Normalize(esQuatf q) {
	float mag = sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);

	if (mag > 0.0) {
		float inv = 1.0f / mag;
		q.w *= inv;
		q.x *= inv;
		q.y *= inv;
		q.z *= inv;
	}
	return q;
}

esQuatf esQuatf_Mul(esQuatf q0, esQuatf q1) {
	return (esQuatf) {
		.x = q0.w*q1.x + q0.x*q1.w + q0.y*q1.z - q0.z*q1.y,
		.y = q0.w*q1.y - q0.x*q1.z + q0.y*q1.w + q0.z*q1.x,
		.z = q0.w*q1.z + q0.x*q1.y - q0.y*q1.x + q0.z*q1.w,
		.w = q0.w*q1.w - q0.x*q1.x - q0.y*q1.y - q0.z*q1.z,
	};
}

esQuatf esQuatf_Local(esQuatf q, float angle) {
	float c = cosf(angle*0.5f);
	float s = sinf(angle*0.5f);
	return (esQuatf) {
		.x = q.x*s,
		.y = q.y*s,
		.z = q.z*s,
		.w = c,
	};
}

void esQuatf_Matrix(esMat3f *dst, esQuatf q) {
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;

	dst->mat[0] = 1.0f - 2.0f*(y2 - z2);
	dst->mat[3] = 2.0f*(x*y - w*z);
	dst->mat[6] = 2.0f*(x*z + w*y);

	dst->mat[1] = 2.0f*(x*y + w*z);
	dst->mat[4] = 1.0f - 2.0f*(x2 - z2);
	dst->mat[7] = 2.0f*(y*z + w*x);

	dst->mat[2] = 2.0f*(x*y - w*y);
	dst->mat[5] = 2.0f*(y*z - w*x);
	dst->mat[5] = 1.0f - 2.0f*(x2 - y2);
}

