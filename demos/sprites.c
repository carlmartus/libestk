#include "estk.h"
#include <GL/glew.h>
#include <SDL/SDL.h>

static esShader shad;
static esTexture tex;

static void loop_frame(float time) {
	static float accu = 0.0f;
	accu += time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad);
	glUniform1i(esShaderUniformGl(&shad, 0), 0);

	es2dSpritesPut(0.0f, 0.0f, accu*0.1f, -2.0f*accu, 0, 0, 0);
	es2dSpritesPut(0.5f, 0.5f, 0.1f, accu, 0, 0, ES_SPRITE_FLIPX);

	esTextureUse(&tex);
	es2dSpritePrepear();
	es2dSpritesRender();

	esGameGlSwap();

	if (accu > 2.0f) {
		esGameLoopQuit();
	}
}

static void loop_exit() {
	es2dSpritesClear();
	esShaderUnload(&shad);
	esTextureUnload(&tex);
}

int main() {

	esGameInit(400, 300);
	esLogVersion();

	if (!esTextureLoad(&tex, "demores/img.png", TEX_LINEAR, TEX_LINEAR)) {
		printf("Cannot load image!\n");
		return 1;
	}

	if (!esShaderDual(&shad, "demores/sprites.vert", "demores/sprites.frag")) {
		esLog(ES_ERRO, "Cannot load shaders!\n");
		return 1;
	}

	if (!esShaderUniformRegister(&shad, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	es2dSpritesInit(1, 100);

	glClearColor(0.3, 0.4, 0.5, 1.0);

	// Transperancy
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esGameLoop(loop_frame, loop_exit, 0);
	return 0;
}

