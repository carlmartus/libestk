#include "estk.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

static esShader shader;
static esGeo geo;
static esGeoBuf geoBuf;

static void frame(float fr) {
	static float time = 0.0f;
	time += fr;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static const esVec3f points[] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
	};

	esQuatf q = { };
	int id = (int) (time*0.25f);
	switch (id % 3) {
		default :
		case 0 : q.x = 1.0f; break;
		case 1 : q.y = 1.0f; break;
		case 2 : q.z = 1.0f; break;
	}

	esQuatf local = esQuatf_local(q, time);

	esMat3f mat;
	q = esQuatf_mul(local, q);
	esQuatf_matrix(&mat, q);

	int i;
	esVec3f p[3];
	for (i=0; i<3; i++) {
		p[i] = esMat3f_MulVec3f(&mat, points[i]);
	}

	//esLog(ES_INFO, "P[0] %f %f %f", p[0].x, p[0].y, p[0].z);
	//esLog(ES_INFO, "P[1] %f %f %f", p[1].x, p[1].y, p[1].z);
	//esLog(ES_INFO, "P[2] %f %f %f", p[2].x, p[2].y, p[2].z);
	esGeoBuf_array(&geoBuf, p, sizeof(p), GEOBUF_STREAM);
	esGeo_renderArray(&geo, 3);

	esGame_glSwap();
}

static void loop_exit(void) {
	esGeoBuf_free(&geoBuf);
}

static void callback_quit(int sdlkey, int down) {
	esGame_loopQuit();
}

int main() {
	esGame_init(400, 300);
	esLogVersion();

	// Geometric buffer
	esGeoBuf_create(&geoBuf);
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geoBuf, GEODATA_FLOAT,
			3, 0, sizeof(esVec3f), ES_FALSE);

	// Shader dual
	if (!esShader_dual(&shader,
				"demores/cam.vert", "demores/red.frag")) {
		esLog(ES_ERRO, "Can't load shaders");
	}

	esShader_use(&shader);

	if (!esShader_uniformRegister(&shader, 0, "un_view")) {
		esLog(ES_ERRO, "Cannot get uniform");
		return 1;
	}

	esMat4f mat;
	esVec3f cam_ey = { -2.0f, -1.5f, 2.5f };
	esVec3f cam_at = { 0.0f, 0.0f, 0.0f, };
	esVec3f cam_up = { 0.0f, 0.0f, 1.0f };
	esProj_perspective(&mat,
			1.3f, 1.333f, 0.1f, 20.0f,
			cam_ey, cam_at, cam_up);
	glUniformMatrix4fv(esShader_uniformGl(&shader, 0),
			1, 0, (const float*) &mat);

	// Loop
	glClearColor(0.3, 0.4, 0.6, 1.0);
	esGame_registerKey(SDLK_q, callback_quit);
	esGame_loop(frame, loop_exit, 0);

	return 0;
}

