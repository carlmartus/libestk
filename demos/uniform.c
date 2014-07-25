#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGameInit(400, 300);
	esLogVersion();

	esShader shad;
	if (!esShaderDual(&shad, "demores/uni.vert", "demores/uni.frag")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (!esShaderUniformRegister(&shad, 0, "un_base")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShaderUse(&shad);
	glUniform4f(esShaderUniformGl(&shad, 0), 0.0f, 1.0f, 0.0f, 1.0f); // Green

	static const float red_lo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	esGeoBuf geobuf;
	esGeoBufCreate(&geobuf);
	esGeoBufArray(&geobuf, red_lo, sizeof(red_lo), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 1);
	esGeoPoint(&geo, 0, &geobuf, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad);
	esGeoRenderArray(&geo, 3);

	esGameGlSwap();

	esGeoBufDelete(&geobuf);
	esShaderUnload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

