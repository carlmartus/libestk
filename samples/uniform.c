#include <stdio.h>
#include "estk.h"

const char *vert = ES_SHADER_SOURCE(
precision mediump float;

attribute vec3 inVertex;
uniform vec4 unBase;
varying vec4 vaBase;

void main() {
	vaBase = unBase;
	gl_Position = vec4(inVertex, 1);
});

const char *frag = ES_SHADER_SOURCE(
precision mediump float;

varying vec4 vaBase;

void main() {
	gl_FragColor = vaBase;
});


int main(int argc, char **argv) {

	esGame_init("Uniform sample", 400, 300);
	esLogVersion();

	esShader shad;
	esShaderAttrib shadAttribs[] = {
		{ 0, "inVertex" },
	};
	if (!esShader_dualText(&shad, vert, frag, shadAttribs, 1)) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "unBase")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShader_use(&shad);
	esLog(ES_INFO, "Uniform %p", glUniform4f);
	glUniform4f(esShader_uniformGl(&shad, 0), 0.0f, 1.0f, 0.0f, 1.0f); // Green

	static const float red_lo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	esGeoBuf geobuf;
	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, red_lo, sizeof(red_lo), ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geobuf, ES_DATA_FLOAT, 2, 0, 0, ES_FALSE);

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

