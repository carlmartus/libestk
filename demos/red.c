#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGame_init(400, 300);
	esLogVersion();

	esShader shad;
	if (!esShader_dual(&shad, "demores/red.vert", "demores/red.frag")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	static const float red_lo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	esGeoBuf geoBuf;
	esGeoBuf_create(&geoBuf);
	esGeoBuf_array(&geoBuf, red_lo, sizeof(red_lo), GEOBUF_STATIC);

	static uint16_t red_el[] = {
		0, 1, 2,
	};

	esGeoBuf geoElements;
	esGeoBuf_create(&geoElements);
	esGeoBuf_element(&geoElements, red_el, sizeof(red_el), GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geoBuf, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	esGeo_renderElements(&geo, &geoElements, GEODATA_USHORT, 3);
	//esGeoRenderArray(&geo, 3);
	esCheckGlError();

	esGame_glSwap();
	esGeoBuf_free(&geoElements);
	esGeoBuf_free(&geoBuf);
	esShader_free(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

