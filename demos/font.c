#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGame_init(400, 300);
	esLogVersion();

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esTexture bitmap;
	if (!esTexture_load(&bitmap,
				"demores/font.png", TEX_LINEAR, TEX_LINEAR)) {
		printf("Cannot open bitmap font\n");
		return 1;
	}

	esShader shad;
	if (!esShader_dual(&shad,
				"demores/font.vert",
				"demores/font.frag")) {
		printf("Cannot open shader\n");
		return 1;
	}

	if (!esShader_uniformRegister(&shad, 0, "un_mvp")) {
		printf("Cannot register mvp uniform\n");
		return 1;
	} else {
		esShader_use(&shad);
		float mat[16];
		esProj_ortho(mat, -10.0f, 10.0f, 10.0f, -10.0f);
		glUniformMatrix4fv(esShader_uniformGl(&shad, 0), 1, 0, mat);
	}

	if (!esShader_uniformRegister(&shad, 1, "un_tex0")) {
		printf("Cannot register texture uniform\n");
		return 1;
	} else {
		glUniform1i(esShader_uniformGl(&shad, 1), 0);
	}

	esFont font;
	esFont_create(&font, &bitmap, &shad, 0, 1, 0);
	esFont_addText(&font, -4.0f, -2.0f, "Hejsan!");

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esFont_render(&font);
	esFont_clearBuf(&font);

	esGame_glSwap();

	esFont_delete(&font);
	esShader_free(&shad);
	esTexture_free(&bitmap);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

