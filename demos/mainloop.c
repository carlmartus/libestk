#include <stdio.h>
#include <GL/glew.h>
#include <math.h>
#include <SDL/SDL.h>
#include "estk.h"

static esShader shad;
static esGeo geo;
static esGeoBuf geobuf;

static void frame(float time) {
	static int frame_count = 0;

	esLog(ES_INFO, "Frame %3.3f", time);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);

	float v = 0.01f * (float) frame_count;
	esVec3f cam_ey = { 3.0f*cosf(v*4.0f), 3.0f*sinf(v*4.0f), 1.0f };
	esVec3f cam_at = { 0.0f, 0.0f, 0.0f };
	esVec3f cam_up = { 0.0f, 0.0f, 1.0f };

	esMat4f mat;
	esProj_perspective(&mat, 0.9f+v, 1.333f, 0.1f, 20.0f, cam_ey, cam_at, cam_up);

	glUniformMatrix4fv(esShader_uniformGl(&shad, 0),
			1, 0, (const float*) &mat);

	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	if (++frame_count > 100) esGame_loopQuit();
}

static void loop_exit(void) {
	esLog(ES_INFO, "Good bye!");
	esGeoBuf_free(&geobuf);
	esShader_free(&shad);
	SDL_Quit();
}

int main() {
	esGame_init(400, 300);
	esLogVersion();

	esShaderAttrib shadAttribs[] = {
		{ 0, "in_vertex" },
	};
	if (!esShader_dualFile(&shad, "demores/cam.vert", "demores/red.frag",
				shadAttribs, 1)) {
		esLog(ES_ERRO, "Cannot load shaders!");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "un_view")) {
		esLog(ES_ERRO, "Cannot get uniform");
		return 1;
	}

	static const float lo[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, lo, sizeof(lo), ES_GEOBUF_STATIC);

	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geobuf, ES_GEODATA_FLOAT, 3, 0, 0, ES_FALSE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	esGame_loop(frame, loop_exit, 0);
	return 0;
}

