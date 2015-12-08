#include <stdio.h>
#include <GL/glew.h>
#include "estk.h"

int main() {

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

	static const struct data {
		float x, y;
		unsigned char r, g, b, a;
	} data[] = {
		{ 0.0f, 0.0f, 0xff, 0, 0, 0xff },
		{ 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{ 0.0f, 1.0f, 0, 0, 0xff, 0xff },
	};

	esGeoBuf geobuf;
	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, data, sizeof(data), ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 2);
	esGeo_point(&geo, 0, &geobuf, ES_DATA_FLOAT, 2, 0, sizeof(struct data), ES_FALSE);
	esGeo_point(&geo, 1, &geobuf, ES_DATA_UBYTE, 4, 8, sizeof(struct data), ES_TRUE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	esGeo_renderArray(&geo, 3);

	esGame_glSwap();

	esGeoBuf_free(&geobuf);
	esShader_free(&shad);

	esGame_delay(800);
	esGame_quit();
	return 0;
}

