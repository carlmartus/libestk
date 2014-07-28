#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main() {

	esGameInit(400, 300);
	esLogVersion();

	esTexture tex;
	if (!esTextureLoad(&tex, "demores/img.png", TEX_LINEAR, TEX_NONE)) {
		printf("Cannot load image!\n");
		return 1;
	}

	esShader shad;
	if (!esShader_Dual(&shad, "demores/img.vert", "demores/img.frag")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (!esShader_UniformRegister(&shad, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShader_Use(&shad);
	glUniform1i(esShader_UniformGl(&shad, 0), 0);

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
	esGeoBufCreate(&geobuf);
	esGeoBufArray(&geobuf, data, sizeof(data), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 3);
	esGeoPoint(&geo, 0, &geobuf, GEODATA_FLOAT, 2,  0, sizeof(struct data), ES_FALSE);
	esGeoPoint(&geo, 1, &geobuf, GEODATA_FLOAT, 2,  8, sizeof(struct data), ES_TRUE);
	esGeoPoint(&geo, 2, &geobuf, GEODATA_UBYTE, 4, 16, sizeof(struct data), ES_TRUE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esShader_Use(&shad);
	esTextureUse(&tex);
	esGeoRenderArray(&geo, 3);

	esGameGlSwap();

	esTextureUnload(&tex);
	esGeoBufDelete(&geobuf);
	esShader_Unload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

