#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	// Method 1
	esShader shad1;
	if (!esShader_dual(&shad1, "demores/red.vert", "demores/red.frag")) {
		esLog(ES_ERRO, "Cannot load shaders using method 1!");
		return 1;
	}
	esShader_free(&shad1);

	// Method 2
	esShader shad2;
	esShader_reset(&shad2);
	if (!(
				esShader_load(&shad2, ES_SHADER_VERT, "demores/red.vert") &&
				esShader_load(&shad2, ES_SHADER_FRAG, "demores/red.frag") &&
				esShader_compile(&shad2))) {
		esLog(ES_ERRO, "Cannot load shaders using method 2!");
		return 1;
	}
	esShader_free(&shad2);

	SDL_Quit();
	return 0;
}

