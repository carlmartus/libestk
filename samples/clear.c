#include <stdio.h>
#include <GL/gl.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	glClearColor(0.3, 0.4, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esGame_glSwap();

	esGame_delay(800);
	esGame_quit();
	return 0;
}

