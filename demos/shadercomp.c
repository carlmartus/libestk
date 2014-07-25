#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGameInit(400, 300);
	esLogVersion();

	esShader shad;
	if (!esShaderDual(&shad, "demores/red.vert", "demores/red.frag")) {
		printf("Cannot load shaders!\n");
		return 1;
	}
	esShaderUnload(&shad);

	SDL_Quit();
	return 0;
}

