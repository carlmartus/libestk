#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main() {

	esGame_init(400, 300);
	esLogVersion();

	esTexture tex;
	if (!esTexture_load(&tex, "demores/img.png", TEX_LINEAR, TEX_NONE)) {
		printf("Cannot load image!\n");
		return 1;
	}

	esShader shad;
	esShaderAttrib shadAttribs[] = {
		{ 0, "in_vertex" },
		{ 1, "in_uv" },
		{ 2, "in_col" },
	};
	if (!esShader_dual(&shad, "demores/img.vert", "demores/img.frag",
				shadAttribs, 3)) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShader_use(&shad);
	glUniform1i(esShader_uniformGl(&shad, 0), 0);

	static const struct data {
		float x, y;
		float u, v;
		unsigned char r, g, b, a;
	} data[] = {
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0xff, 0, 0, 0xff },
		{ 1.0f, 0.0f, 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{ 0.0f, 1.0f, 0.0f, 1.0f, 0, 0, 0xff, 0xff },
	};

	esGeoBuf geobuf;
	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, data, sizeof(data), GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 3);
	esGeo_point(&geo, 0, &geobuf, GEODATA_FLOAT, 2,  0, sizeof(struct data), ES_FALSE);
	esGeo_point(&geo, 1, &geobuf, GEODATA_FLOAT, 2,  8, sizeof(struct data), ES_TRUE);
	esGeo_point(&geo, 2, &geobuf, GEODATA_UBYTE, 4, 16, sizeof(struct data), ES_TRUE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esShader_use(&shad);
	esTexture_use(&tex);
	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	esTexture_free(&tex);
	esGeoBuf_free(&geobuf);
	esShader_free(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

