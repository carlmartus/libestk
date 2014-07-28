#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGameInit(400, 300);
	esLogVersion();

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esTexture bitmap;
	if (!esTextureLoad(&bitmap,
				"demores/font.png", TEX_LINEAR, TEX_LINEAR)) {
		printf("Cannot open bitmap font\n");
		return 1;
	}

	esShader shad;
	if (!esShader_Dual(&shad,
				"demores/font.vert",
				"demores/font.frag")) {
		printf("Cannot open shader\n");
		return 1;
	}

	if (!esShader_UniformRegister(&shad, 0, "un_mvp")) {
		printf("Cannot register mvp uniform\n");
		return 1;
	} else {
		esShader_Use(&shad);
		float mat[16];
		esProjOrtho(mat, -10.0f, 10.0f, 10.0f, -10.0f);
		glUniformMatrix4fv(esShader_UniformGl(&shad, 0), 1, 0, mat);
	}

	if (!esShader_UniformRegister(&shad, 1, "un_tex0")) {
		printf("Cannot register texture uniform\n");
		return 1;
	} else {
		glUniform1i(esShader_UniformGl(&shad, 1), 0);
	}

	esFont font;
	esFontCreate(&font, &bitmap, &shad, 0, 1, 0);
	esFontAddText(&font, -4.0f, -2.0f, "Hejsan!");

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esFontRender(&font);
	esFontClearBuf(&font);

	esGameGlSwap();

	esFontDelete(&font);
	esShader_Unload(&shad);
	esTextureUnload(&bitmap);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

