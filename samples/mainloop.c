#include <stdio.h>
#include <GL/glew.h>
#include <math.h>
#include <assert.h>
#include "estk.h"
#include "resources/mesh_jeep.h"

//=============================================================================
// GLSL program
//=============================================================================

static const char glslMesh_vert[] = ES_SHADER_SOURCE(
	attribute vec3 in_location;
	attribute vec4 in_color;

	varying vec4 va_color;

	uniform mat4 un_view;

	void main() {
		va_color = in_color;
		gl_Position = un_view*vec4(in_location, 1.0);
	});

static const char glslMesh_frag[] = ES_SHADER_SOURCE(
	varying vec4 va_color;

	void main() {
		gl_FragColor = va_color;
	});

//=============================================================================
// Internals
//=============================================================================

static esShader shad;
static esGeo geo;
static esGeoBuf geobuf;

static void frame(float time);
static void loop_exit(void);

//=============================================================================
// Program
//=============================================================================

int main() {
	esGame_init(400, 300);
	esLogVersion();

	esShaderAttrib shadAttribs[] = {
		{ 0, "in_location" },
		{ 1, "in_color" },
	};

	glEnable(GL_DEPTH_TEST);

	assert(esShader_dualText(&shad, glslMesh_vert, glslMesh_frag,
				shadAttribs, 1) == ES_OK);
	assert(esShader_uniformRegister(&shad, 0, "un_view") == ES_OK);

	esGeoBuf_create(&geobuf);
	esGeoBuf_array(&geobuf, vertData, sizeof(vertData), ES_GEOBUF_STATIC);

	esGeo_reset(&geo, 2);
	esGeo_point(&geo, 0, &geobuf,
			ES_DATA_FLOAT, 3, 0, sizeof(Vertex), ES_FALSE);
	esGeo_point(&geo, 1, &geobuf,
			ES_DATA_UBYTE, 4, 12, sizeof(Vertex), ES_TRUE);

	glClearColor(0.3, 0.4, 0.6, 1.0);
	esGame_loop(frame, loop_exit, 0);
	return 0;
}


static void frame(float time) {
	static int frame_count = 0;

	//esLog(ES_INFO, "Frame %3.3f", time);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);

	float v = 0.01f * (float) frame_count;
	esVec3f cam_ey = { 3.0f*cosf(v*4.0f), 3.0f*sinf(v*4.0f), 3.0f };
	esVec3f cam_at = { 0.0f, 0.0f, 0.0f };
	esVec3f cam_up = { 0.0f, 0.0f, 1.0f };

	esMat4f mat;
	esProj_perspective(&mat, 0.9f+v, 1.333f, 0.1f, 20.0f,
			cam_ey, cam_at, cam_up);

	glUniformMatrix4fv(esShader_uniformGl(&shad, 0),
			1, 0, (const float*) &mat);

	esGeo_renderArray(&geo, sizeof(vertData) / sizeof(vertData[0]));

	esGame_glSwap();

	if (++frame_count > 100) esGame_loopQuit();
}

static void loop_exit(void) {
	esLog(ES_INFO, "Good bye!");
	esGeoBuf_free(&geobuf);
	esShader_free(&shad);
	esGame_quit();
}

