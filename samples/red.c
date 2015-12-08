#include <stdio.h>
#include <GL/glew.h>
#include "estk.h"

static const char *glslRed_vert = ES_SHADER_SOURCE(
	precision mediump float;
	attribute vec3 in_vertex;

	void main() {
		gl_Position = vec4(in_vertex, 1.0);
	});

static const char *glslRed_frag = ES_SHADER_SOURCE(
	precision mediump float;

	void main() {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	});

int main() {

	esGame_init(400, 300);
	esLogVersion();

	esShader shad;
	esShaderAttrib shadAttribs[] = {
		{ 0, "in_vertex" },
	};
	if (!esShader_dualText(&shad, glslRed_vert, glslRed_frag,
				shadAttribs, 1)) {
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
	esGeoBuf_array(&geoBuf, red_lo, sizeof(red_lo), ES_GEOBUF_STATIC);

	static uint16_t red_el[] = {
		0, 1, 2,
	};

	esGeoBuf geoElements;
	esGeoBuf_create(&geoElements);
	esGeoBuf_element(&geoElements, red_el, sizeof(red_el), ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &geoBuf, ES_DATA_FLOAT, 2, 0, 0, ES_FALSE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	esGeo_renderElements(&geo, &geoElements, ES_DATA_USHORT, 3);
	//esGeoRenderArray(&geo, 3);
	esCheckGlError();

	esGame_glSwap();
	esGeoBuf_free(&geoElements);
	esGeoBuf_free(&geoBuf);
	esShader_free(&shad);

	esGame_delay(800);
	esGame_quit();
	return 0;
}

