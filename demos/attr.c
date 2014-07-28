#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	esShader shad;
	if (!esShader_dual(&shad, "demores/attr.vert", "demores/attr.frag")) {
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
	esGeoBuf_create(&bufLo);
	esGeoBuf_array(&bufLo, attrLo, sizeof(attrLo), GEOBUF_STATIC);

	esGeoBuf bufCo;
	esGeoBuf_create(&bufCo);
	esGeoBuf_array(&bufCo, attrCo, sizeof(attrCo), GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 2);
	esGeo_point(&geo, 0, &bufLo, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);
	esGeo_point(&geo, 1, &bufCo, GEODATA_UBYTE, 4, 0, 0, ES_TRUE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	esGeoBuf_free(&bufLo);
	esGeoBuf_free(&bufCo);
	esShader_free(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

