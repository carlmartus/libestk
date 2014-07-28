#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGame_init(400, 300);
	esLogVersion();

	esShader shad_red;
	if (!esShader_dual(&shad_red,
				"demores/red.vert", "demores/red.frag")) {
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

	esShader shad_tex;
	if (!esShader_dual(&shad_tex,
				"demores/img.vert", "demores/img.frag")) {
		printf("Cannot load shad_reders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad_tex, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShader_use(&shad_tex);
	glUniform1i(esShader_uniformGl(&shad_tex, 0), 0);

	esFrameBuffer fb;
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
	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad_red);
	esGeo_renderArray(&cube, 6);
	esFb_unSet();

	// Render geometry
	esFb_bind(&fb);
	esShader_use(&shad_tex);
	esGeo_renderArray(&texgeo, 6);

	esGame_glSwap();
	esFb_free(&fb);
	esGeoBuf_free(&cubebuf);
	esShader_free(&shad_red);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}


