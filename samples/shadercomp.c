#include <stdio.h>
#include <GL/glew.h>
#include "estk.h"

int main(int argc, char **argv) {

	esGame_init(400, 300);
	esLogVersion();

	esShaderAttrib shadAttribs[] = {
		{ 0, "in_vertex" },
	};

	// Method 1
	esShader shad1;
	if (!esShader_dualFile(&shad1,
				"samples/resources/red.vert", "samples/resources/red.frag",
				shadAttribs, 1)) {
		esLog(ES_ERRO, "Cannot load shaders using method 1!");
		return 1;
	}
	esShader_free(&shad1);

	// Method 2
	esShader shad2;
	esShader_reset(&shad2);
	if (!(
				esShader_loadFile(&shad2, ES_SHADER_VERT, "samples/resources/red.vert") &&
				esShader_loadFile(&shad2, ES_SHADER_FRAG, "samples/resources/red.frag") &&
				esShader_compile(&shad2, shadAttribs, 1))) {
		esLog(ES_ERRO, "Cannot load shaders using method 2!");
		return 1;
	}
	esShader_free(&shad2);

	esGame_quit();
	return 0;
}

