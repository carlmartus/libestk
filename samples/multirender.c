#include "estk.h"
#include <GL/glew.h>
#include <assert.h>

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
	uniform sampler2D un_tex0;
	void main() {
		gl_FragData[0] = texture2D(un_tex0, va_uv);

		float r = length(va_uv - vec2(0.5, 0.5));
		gl_FragData[1] = vec4(va_uv*r, 0, 0);
	});

static const char mergeSrc_vert[] = ES_SHADER_SOURCE(
	attribute vec2 in_uv;
	varying vec2 va_uv;
	void main() {
		va_uv = in_uv;
		gl_Position = vec4(2*(in_uv - vec2(0.5, 0.5)), 0, 1);
	});

static const char mergeSrc_frag[] = ES_SHADER_SOURCE(
	uniform sampler2D un_tex0;
	uniform sampler2D un_tex1;
	varying vec2 va_uv;
	void main() {
		vec2 displace = texture2D(un_tex1, va_uv).xy;
		gl_FragColor =texture2D(un_tex0, displace);
			
	});

//=============================================================================
// Geometry
//=============================================================================

static const int8_t cubeLoc[] = {
	-1, -1,		 1, -1,		-1,  1,
	 1, -1,		 1,  1,		-1,  1,
};

static const int8_t cubeUvs[] = {
	0, 0,		 1, 0,		0,  1,
	1, 0,		 1, 1,		0,  1,
};

static const uint8_t mergeUvs[] = {
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
	esShader programCube;
	esShaderAttrib programCubeAttribs[] = {
		{ 0, "in_vertex" },
		{ 1, "in_uv" },
	};

	assert(esShader_dualText(&programCube,
				redSrc_vert, redSrc_frag,
				programCubeAttribs, 1) == ES_OK);
	assert(esShader_uniformRegister(&programCube, 0, "un_tex0") == ES_OK);
	esShader_use(&programCube);
	esCheckGlError();
	glUniform1i(esShader_uniformGl(&programCube, 0), 0);
	esCheckGlError();

	esShader programMerge;
	esShaderAttrib programMergeAttribs[] = {
		{ 0, "in_uv" },
	};

	assert(esShader_dualText(&programMerge,
				mergeSrc_vert, mergeSrc_frag,
				programMergeAttribs, 1) == ES_OK);
	assert(esShader_uniformRegister(&programMerge, 0, "un_tex0") == ES_OK);
	assert(esShader_uniformRegister(&programMerge, 1, "un_tex1") == ES_OK);
	esShader_use(&programMerge);
	esCheckGlError();
	glUniform1i(esShader_uniformGl(&programMerge, 0), 0);
	glUniform1i(esShader_uniformGl(&programMerge, 1), 1);
	esCheckGlError();

	// Geometry
	esGeoBuf bufCubeLoc, bufCubeUvs, bufMergeUvs;

	esGeoBuf_create(&bufCubeLoc);
	esGeoBuf_array(&bufCubeLoc, cubeLoc, sizeof(cubeLoc),
			ES_GEOBUF_STATIC);

	esGeoBuf_create(&bufCubeUvs);
	esGeoBuf_array(&bufCubeUvs, cubeUvs, sizeof(cubeUvs),
			ES_GEOBUF_STATIC);

	esGeo geoCube;
	esGeo_reset(&geoCube, 2);
	esGeo_point(&geoCube, 0, &bufCubeLoc, ES_DATA_BYTE, 2, 0, 0, ES_FALSE);
	esGeo_point(&geoCube, 1, &bufCubeUvs, ES_DATA_UBYTE, 2, 0, 0, ES_FALSE);

	esGeoBuf_create(&bufMergeUvs);
	esGeoBuf_array(&bufMergeUvs, mergeUvs, sizeof(mergeUvs),
			ES_GEOBUF_STATIC);

	esGeo geoMerge;
	esGeo_reset(&geoMerge, 1);
	esGeo_point(&geoMerge, 0, &bufMergeUvs, ES_DATA_UBYTE, 2, 0, 0, ES_FALSE);

	// Image
	char imageBuf[64*64*2];
	generateImage(imageBuf, 64, 64);

	esTexture tex;
	esTexture_createRaw(&tex, 2, 64, 64, ES_DATA_UBYTE, imageBuf,
			ES_TEX_NONE, ES_TEX_NONE);

	// Multi render
	esMultiRender mr;
	if (esMultiRender_create(&mr, 400, 300, ES_TEX_LINEAR, ES_TEX_LINEAR,
				2, 3, 2) != ES_OK) {
		esLog(ES_ERRO, "Error creating multi render!\n");
		return 1;
	}

	// Render
	esMultiRender_bind(&mr);
	esCheckGlError();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	esCheckGlError();


	esTexture_use(&tex);
	esShader_use(&programCube);
	esGeo_renderArray(&geoCube, 6);
	esCheckGlError();

	esMultiRender_unBind();
	esGame_setViewport();

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	esCheckGlError();

	esMultiRender_bindAllTextures(&mr);
	esCheckGlError();

	esShader_use(&programMerge);
	esGeo_renderArray(&geoMerge, 6);
	esCheckGlError();

	esGame_glSwap();

	// Free
	esGeoBuf_free(&bufCubeLoc);
	esGeoBuf_free(&bufCubeUvs);
	esGeoBuf_free(&bufMergeUvs);
	esShader_free(&programCube);

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
			*dst++ = ((x >> GEN_SHIFT) & 1) ^ ((y >> GEN_SHIFT) & 1)? 0xff:0;
			*dst++ = y + ((x & ((1 << GEN_SHIFT) - 1)) << 4);
		}
	}

#undef GEN_SHIFT
}

