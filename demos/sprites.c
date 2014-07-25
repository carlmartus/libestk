#include "estk.h"
#include <GL/glew.h>
#include <SDL/SDL.h>

int main() {

	esGameInit(400, 300);
	esLogVersion();

	esTexture tex;
	if (!esTextureLoad(&tex, "demores/img.png", TEX_LINEAR, TEX_LINEAR)) {
		printf("Cannot load image!\n");
		return 1;
	}

	esShader shad;
	if (!esShaderDual(&shad, "demores/sprites.vert", "demores/sprites.frag")) {
		esLog(ES_ERRO, "Cannot load shaders!\n");
		return 1;
	}

	if (!esShaderUniformRegister(&shad, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	es2dSpritesInit(2, 100);

	glClearColor(0.3, 0.4, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esShaderUse(&shad);
	glUniform1i(esShaderUniformGl(&shad, 0), 0);

	es2dSpritesPut(0.0f, 0.0f, 0.1f, 0, 1);
	es2dSpritesPut(0.5f, 0.5f, 0.2f, 1, 0);

	esTextureUse(&tex);
	es2dSpritePrepear();
	es2dSpritesRender();

	esGameGlSwap();

	es2dSpritesClear();
	esShaderUnload(&shad);
	esTextureUnload(&tex);

	SDL_Delay(800);
	SDL_Quit();

	return 0;
}

