#include "estk.h"
#include <GL/glew.h>

//=============================================================================
// Shaders
//=============================================================================

static const char redSrc_vert[] = ES_SHADER_SOURCE(
	attribute vec2 in_vertex;
	attribute vec2 in_uv;
	varying vec2 va_uv;
	void main() {
		va_uv = in_uv;
		gl_Position = vec4(in_vertex, 0, 1);
	});

static const char redSrc_frag[] = ES_SHADER_SOURCE(
	varying vec2 va_uv;
	void main() {
		gl_FragData[0] = vec4(va_uv, 0, 1);
		gl_FragData[1] = vec4(0, 1, 1, 1);
	});

//=============================================================================
// Geometry
//=============================================================================

static const int8_t triangleLoc[] = {
	-1, -1,		 1, -1,		-1,  1,
	 1, -1,		 1,  1,		-1,  1,
};

static const int8_t triangleUv[] = {
	0, 0,		 1, 0,		0,  1,
	1, 0,		 1, 1,		0,  1,
};

//=============================================================================
// Program
//=============================================================================

static void generateImage(void *ptr, unsigned w, unsigned h);

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	// Shader
	esShader red;
	esShaderAttrib redAttribs[] = {
		{ 0, "in_vertex" },
		{ 1, "in_uv" },
	};

	if (!esShader_dualText(&red, redSrc_vert, redSrc_frag, redAttribs, 1)) {
		esLog(ES_ERRO, "Cannot load shaders!\n");
		return 1;
	}

	// Geometry
	esGeoBuf bufTriangleLoc, bufTriangleUv;

	esGeoBuf_create(&bufTriangleLoc);
	esGeoBuf_array(&bufTriangleLoc, triangleLoc, sizeof(triangleLoc),
			ES_GEOBUF_STATIC);

	esGeoBuf_create(&bufTriangleUv);
	esGeoBuf_array(&bufTriangleUv, triangleUv, sizeof(triangleUv),
			ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 2);
	esGeo_point(&geo, 0, &bufTriangleLoc, ES_DATA_BYTE, 2, 0, 0, ES_FALSE);
	esGeo_point(&geo, 1, &bufTriangleUv, ES_DATA_UBYTE, 2, 0, 0, ES_FALSE);

	// Image
	char imageBuf[64*64*2];
	generateImage(imageBuf, 64, 64);

	esTexture tex;
	esTexture_createRaw(&tex, 2, 64, 64, ES_DATA_UBYTE, imageBuf,
			ES_TEX_LINEAR, ES_TEX_LINEAR);

	// Multi render
	esMultiRender mr;
	if (esMultiRender_create(&mr, 400, 300, ES_TEX_LINEAR, ES_TEX_LINEAR,
				2, 2, 3) != ES_OK) {
		esLog(ES_ERRO, "Error creating multi render!\n");
		return 1;
	}

	// Render
	esMultiRender_bind(&mr);
	esMultiRender_unBind();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	esShader_use(&red);
	esGeo_renderArray(&geo, 6);

	/*
	esMultiRender_unBind();
	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*/

	esGame_glSwap();

	// Free
	esGeoBuf_free(&bufTriangleLoc);
	esShader_free(&red);

	esGame_delay(800);
	esGame_quit();
	return 0;
}


static void generateImage(void *ptr, unsigned w, unsigned h) {
#define GEN_SHIFT 3
	int x, y;
	unsigned char *dst = ptr;

	for (y=0; y<h; y++) {
		for (x=0; x<w; x++) {
			*dst++ = ((x >> GEN_SHIFT) & 1) ^ ((y >> GEN_SHIFT) & 1);
			*dst++ = x & ((1 << GEN_SHIFT) - 1);
		}
	}

#undef GEN_SHIFT
}

