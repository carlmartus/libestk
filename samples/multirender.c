#include "estk.h"
#include <GL/glew.h>

//=============================================================================
// Shaders
//=============================================================================

static const char redSrc_vert[] = ES_SHADER_SOURCE(
	attribute vec2 in_vertex;
	void main() {
		gl_Position = vec4(in_vertex, 0, 1);
	});

static const char redSrc_frag[] = ES_SHADER_SOURCE(
	void main() {
		gl_FragData[0] = vec4(1, 1, 0, 1);
		gl_FragData[1] = vec4(0, 1, 1, 1);
	});

//=============================================================================
// Geometry
//=============================================================================

static const int8_t triangleLocation[] = {
	-1, -1,		 1, -1,		-1,  1,
	 1, -1,		 1,  1,		-1,  1,
};

//=============================================================================
// Program
//=============================================================================

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	// Shader
	esShader red;
	esShaderAttrib redAttribs[] = {
		{ 0, "in_vertex" },
	};

	if (!esShader_dualText(&red, redSrc_vert, redSrc_frag, redAttribs, 1)) {
		esLog(ES_ERRO, "Cannot load shaders!\n");
		return 1;
	}

	// Geometry
	esGeoBuf bufTriangle;
	esGeoBuf_create(&bufTriangle);
	esGeoBuf_array(&bufTriangle, triangleLocation, sizeof(triangleLocation),
			ES_GEOBUF_STATIC);

	esGeo geo;
	esGeo_reset(&geo, 1);
	esGeo_point(&geo, 0, &bufTriangle, ES_DATA_BYTE, 2, 0, 0, ES_FALSE);

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
	esGeoBuf_free(&bufTriangle);
	esShader_free(&red);

	esGame_delay(800);
	esGame_quit();
	return 0;
}

