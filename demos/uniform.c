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
	if (!esShader_dual(&shad, "demores/uni.vert", "demores/uni.frag",
				shadAttribs, 1)) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "un_base")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShader_use(&shad);
	glUniform4f(esShader_uniformGl(&shad, 0), 0.0f, 1.0f, 0.0f, 1.0f); // Green

	static const float red_lo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	esGeoBuf geobuf;
	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, red_lo, sizeof(red_lo), GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geobuf, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	esGeoBuf_free(&geobuf);
	esShader_free(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

