#include "estk.h"
#include <GL/glew.h>
#include <SDL/SDL.h>

static esShader shad;
static esTexture tex;

static void loop_frame(float time) {
	static float accu = 0.0f;
	accu += time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShader_use(&shad);
	glUniform1i(esShader_uniformGl(&shad, 0), 0);

	int step = 3.0f*accu;
	esSprites2d_put(0.0f, 0.0f, accu*0.01f+0.1f, -accu,
			1, 0, step&1?ES_SPRITE_FLIPXY:0);
	esSprites2d_put(0.5f, 0.5f, 0.1f, accu, 0, 1, ES_SPRITE_FLIPX);

	esTexture_use(&tex);
	esSprites2d_prepear();
	esSprites2d_render();

	esGame_glSwap();

	if (accu > 3.0f) {
		esGame_loopQuit();
	}
}

static void loop_exit() {
	esSprites2d_clear();
	esShader_free(&shad);
	esTexture_free(&tex);
}

int main() {

	esGame_init(400, 300);
	esLogVersion();

	if (!esTexture_load(&tex, "demores/img.png", TEX_LINEAR, TEX_LINEAR)) {
		printf("Cannot load image!\n");
		return 1;
	}

	if (!esShader_dual(&shad, "demores/sprites.vert", "demores/sprites.frag")) {
		esLog(ES_ERRO, "Cannot load shaders!\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esSprites2d_init(2, 100);

	glClearColor(0.3, 0.4, 0.5, 1.0);

	// Transperancy
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esGame_loop(loop_frame, loop_exit, 0);
	return 0;
}

