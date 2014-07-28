#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGameInit(400, 300);
	esLogVersion();

	esShader shad;
	if (!esShader_Dual(&shad, "demores/attr.vert", "demores/attr.frag")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	static const float attrLo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	static const unsigned char attrCo[] = {
		0xff, 0, 0, 0xff,
		0, 0xff, 0, 0xff,
		0, 0, 0xff, 0xff,
	};

	esGeoBuf bufLo;
	esGeoBufCreate(&bufLo);
	esGeoBufArray(&bufLo, attrLo, sizeof(attrLo), GEOBUF_STATIC);

	esGeoBuf bufCo;
	esGeoBufCreate(&bufCo);
	esGeoBufArray(&bufCo, attrCo, sizeof(attrCo), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 2);
	esGeoPoint(&geo, 0, &bufLo, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);
	esGeoPoint(&geo, 1, &bufCo, GEODATA_UBYTE, 4, 0, 0, ES_TRUE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_Use(&shad);
	esGeoRenderArray(&geo, 3);

	esGameGlSwap();

	esGeoBufDelete(&bufLo);
	esGeoBufDelete(&bufCo);
	esShader_Unload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

