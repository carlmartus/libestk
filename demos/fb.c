#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGame_init(400, 300);
	esLogVersion();

	esShader shadRed;
	esShaderAttrib shadRedAttribs[] = {
		{ 0, "in_vertex" },
	};
	if (!esShader_dualFile(&shadRed,
				"demores/red.vert", "demores/red.frag",
				shadRedAttribs, 1)) {
		printf("Cannot load shad_reders!\n");
		return 1;
	}

	static const float cube_lo[] = {
		-0.4f, -0.4f,
		 0.4f, -0.3f,
		-0.5f,  0.5f,
		 0.4f, -0.3f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	esShader shadTex;
	esShaderAttrib shadTexAttribs[] = {
		{ 0, "in_vertex" },
		{ 1, "in_uv" },
		{ 2, "in_col" },
	};
	if (!esShader_dualFile(&shadTex,
				"demores/img.vert", "demores/img.frag",
				shadTexAttribs, 3)) {
		printf("Cannot load shad_reders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shadTex, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShader_use(&shadTex);
	glUniform1i(esShader_uniformGl(&shadTex, 0), 0);

	esFb fb;
	if (!esFb_create(&fb, 64, TEX_NONE, TEX_LINEAR)) {
		printf("Cannot create frame buffer\n");
		return 1;
	}

	// Red cube
	esGeoBuf cubebuf;
	esGeoBuf_create(&cubebuf);
	esGeoBuf_array(&cubebuf, cube_lo, sizeof(cube_lo), GEOBUF_STATIC);

	esGeo cube;
	esGeo_reset(&cube, 1);
	esGeo_point(&cube, 0, &cubebuf, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);

	// Textured cube
	static const struct data {
		float x, y;
		float u, v;
		unsigned char r, g, b, a;
	} data[] = {
		{ -1.0f, -1.0f, 0.0f, 0.0f, 0xff, 0, 0, 0xff },
		{  1.0f, -1.0f, 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{ -1.0f,  1.0f, 0.0f, 1.0f, 0, 0, 0xff, 0xff },

		{  1.0f, -1.0f, 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{  1.0f,  1.0f, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff },
		{ -1.0f,  1.0f, 0.0f, 1.0f, 0, 0, 0xff, 0xff },
	};

	esGeoBuf geobuf;
	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, data, sizeof(data), GEOBUF_STATIC);

	esGeo texgeo;
	esGeo_reset(&texgeo, 3);
	esGeo_point(&texgeo, 0, &geobuf, GEODATA_FLOAT, 2,  0,
			sizeof(struct data), ES_FALSE);
	esGeo_point(&texgeo, 1, &geobuf, GEODATA_FLOAT, 2,  8,
			sizeof(struct data), ES_TRUE);
	esGeo_point(&texgeo, 2, &geobuf, GEODATA_UBYTE, 4, 16,
			sizeof(struct data), ES_TRUE);

	// Render to texture
	esFb_set(&fb);
	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shadRed);
	esGeo_renderArray(&cube, 6);
	esFb_unSet();

	// Render geometry
	esFb_bind(&fb);
	esShader_use(&shadTex);
	esGeo_renderArray(&texgeo, 6);

	esGame_glSwap();
	esFb_free(&fb);
	esGeoBuf_free(&cubebuf);
	esShader_free(&shadRed);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}


