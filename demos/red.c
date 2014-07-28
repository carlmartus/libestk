#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGameInit(400, 300);
	esLogVersion();

	esShader shad;
	if (!esShader_Dual(&shad, "demores/red.vert", "demores/red.frag")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	static const float red_lo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	esGeoBuf geoBuf;
	esGeoBufCreate(&geoBuf);
	esGeoBufArray(&geoBuf, red_lo, sizeof(red_lo), GEOBUF_STATIC);

	static uint16_t red_el[] = {
		0, 1, 2,
	};

	esGeoBuf geoElements;
	esGeoBufCreate(&geoElements);
	esGeoBufElement(&geoElements, red_el, sizeof(red_el), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 1);
	esGeoPoint(&geo, 0, &geoBuf, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_Use(&shad);
	esGeoRenderElements(&geo, &geoElements, GEODATA_USHORT, 3);
	//esGeoRenderArray(&geo, 3);
	esCheckGlError();

	esGameGlSwap();
	esGeoBufDelete(&geoElements);
	esGeoBufDelete(&geoBuf);
	esShader_Unload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

