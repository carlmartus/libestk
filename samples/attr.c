#include <stdio.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	esShader shad;
	esShaderAttrib shadAttribs[] = {
		{ 0, "in_vertex" },
		{ 1, "in_col" },
	};
	if (!esShader_dualFile(&shad,
				"samples/resources/attr.vert", "samples/resources/attr.frag",
				shadAttribs, 2)) {
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
	esGeoBuf_array(&bufLo, attrLo, sizeof(attrLo), ES_GEOBUF_STATIC);

	esGeoBuf bufCo;
	esGeoBuf_create(&bufCo);
	esGeoBuf_array(&bufCo, attrCo, sizeof(attrCo), ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 2);
	esGeo_point(&geo, 0, &bufLo, ES_GEODATA_FLOAT, 2, 0, 0, ES_FALSE);
	esGeo_point(&geo, 1, &bufCo, ES_GEODATA_UBYTE, 4, 0, 0, ES_TRUE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	esGeoBuf_free(&bufLo);
	esGeoBuf_free(&bufCo);
	esShader_free(&shad);

	esGame_delay(800);
	esGame_quit();
	return 0;
}

