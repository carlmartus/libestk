#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int main() {

	esGame_init(400, 300);
	esLogVersion();

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

