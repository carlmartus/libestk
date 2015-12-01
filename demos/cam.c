#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	esShader shad;
	esShaderAttrib shadAttribs[] = {
		{ 0, "in_vertex" },
	};
	if (!esShader_dualFile(&shad, "demores/cam.vert", "demores/red.frag",
				shadAttribs, 1)) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "un_view")) {
		printf("Cannot get uniform\n");
		return 1;
	}

	static const float lo[] = {
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		0.0f, 1.0f, -1.0f,
	};

	esGeoBuf geobuf;
	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, lo, sizeof(lo), ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geobuf, ES_GEODATA_FLOAT, 3, 0, 0, ES_FALSE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);

	esMat4f mat;

	esVec3f cam_ey = { -1.0f, -0.8f, 0.5f };
	esVec3f cam_at = { 0.0f, 0.0f, -0.5f, };
	esVec3f cam_up = { 0.0f, 0.0f, 1.0f };
	esProj_perspective(&mat, 1.3f, 1.333f, 0.1f, 20.0f, cam_ey, cam_at, cam_up);
	//esProj_ortho(&mat, 0.0f, 0.0f, 4.0f, 3.0f);
	glUniformMatrix4fv(esShader_uniformGl(&shad, 0),
			1, 0, (const float*) &mat);

	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	esGeoBuf_free(&geobuf);
	esShader_free(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

