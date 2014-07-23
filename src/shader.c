#include "estk.h"
#include <GL/glew.h>

static int checkShader(GLuint id, const char *shaderInfo) {
	GLint result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		char info_buf[500];
		glGetShaderInfoLog(id, sizeof(info_buf)-1, NULL, info_buf);
		esCheckGlError();
		return 1;
	}

	return 0;
}

int loadShader(const char *fileName,
		GLenum shaderType, const char *shaderInfo) {

	char *content = esFileAlloc(fileName);
	if (content == NULL) {
		return 0;
	}

	int shad = glCreateShader(shaderType);
	glShaderSource(shad, 1, (const char**) &content , NULL);
	glCompileShader(shad);
	esCheckGlError();
	free(content);

	if (checkShader(shad, shaderInfo)) return 0;

	esLog(ES_INFO, "Loaded shader %s", fileName);
	return shad;
}

int esShaderLoad(esShader *shader,
		const char *vertFile, const char *fragFile) {
	int idvert = loadShader(vertFile, GL_VERTEX_SHADER, "Vertex shader");
	if (idvert == 0) {
		esLog(ES_ERRO, "Invalid vertex shader file (%s)\n", vertFile);
		return 1;
	}

	int idfrag = loadShader(fragFile, GL_FRAGMENT_SHADER, "Fragment shader");
	if (idfrag == 0) {
		esLog(ES_ERRO, "Invalid fragment shader file (%s)\n", fragFile);
		return 1;
	}

	int program = glCreateProgram();

	glAttachShader(program, idvert);
	esCheckGlError();
	glAttachShader(program, idfrag);
	esCheckGlError();

	glLinkProgram(program);
	esCheckGlError();

	glDeleteShader(idvert);
	glDeleteShader(idfrag);

	shader->glprogram = program;
	return 0;
}

void esShaderUse(const esShader *shader) {
	glUseProgram(shader->glprogram);
}

void esShaderUnload(esShader *shader) {
	glDeleteShader(shader->glprogram);
}

int esShaderUniformRegister(esShader *shader,
		esUniform reg, const char *name) {

	int loc = glGetUniformLocation(shader->glprogram, name);
	if (loc < 0) return 1;

	shader->uniforms[reg] = loc;
	return 0;
}

int esShaderUniformGl(esShader *shader, esUniform reg) {
	return shader->uniforms[reg];
}

