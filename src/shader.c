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

void esShaderReset(esShader *shader) {
	shader->shaderCount = 0;
}

int esShaderLoadFrag(esShader *shader, const char *fragFile) {
	int idFrag = loadShader(fragFile,
			GL_FRAGMENT_SHADER, "Fragment shader");
	if (idFrag == 0) {
		esLog(ES_ERRO, "Invalid fragment shader file (%s)\n", fragFile);
		return 1;
	}
	shader->glShaders[shader->shaderCount++] = idFrag;
	return 0;
}

int esShaderLoadVert(esShader *shader, const char *vertFile) {
	int idVert = loadShader(vertFile,
			GL_VERTEX_SHADER, "Vertex shader");
	if (idVert == 0) {
		esLog(ES_ERRO, "Invalid vertex shader file (%s)\n", vertFile);
		return 1;
	}
	shader->glShaders[shader->shaderCount++] = idVert;
	return 0;
}

int esShaderCompile(esShader *shader) {

	int program = glCreateProgram();

	int i;
	for (i=0; i<shader->shaderCount; i++) {
		glAttachShader(program, shader->glShaders[i]);
		esCheckGlError();
	}

	glLinkProgram(program);
	esCheckGlError();

	for (i=0; i<shader->shaderCount; i++) {
		glDeleteShader(shader->glShaders[i]);
		esCheckGlError();
	}

	shader->glProgram = program;
	return 0;
}

int esShaderDual(esShader *shader,
		const char *vertFile, const char *fragFile) {
	esShaderReset(shader);

	if (esShaderLoadVert(shader, vertFile)) return 1;
	if (esShaderLoadFrag(shader, fragFile)) return 1;
	return esShaderCompile(shader);
}

void esShaderUse(const esShader *shader) {
	glUseProgram(shader->glProgram);
}

void esShaderUnload(esShader *shader) {
	glDeleteShader(shader->glProgram);
}

int esShaderUniformRegister(esShader *shader,
		esUniform reg, const char *name) {

	int loc = glGetUniformLocation(shader->glProgram, name);
	if (loc < 0) return 1;

	shader->uniforms[reg] = loc;
	return 0;
}

int esShaderUniformGl(esShader *shader, esUniform reg) {
	return shader->uniforms[reg];
}

