#include "estk.h"
#include <GL/glew.h>

static int checkShader(GLuint id, const char *shaderInfo) {
	GLint result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		char info_buf[500];
		glGetShaderInfoLog(id, sizeof(info_buf)-1, NULL, info_buf);
		esCheckGlError();
		return ES_FAIL;
	}

	return ES_OK;
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

	if (!checkShader(shad, shaderInfo)) return 0;

	esLog(ES_INFO, "Loaded shader %s", fileName);
	return shad;
}

void esShader_reset(esShader *shader) {
	shader->shaderCount = 0;
}

esErr esShader_load(esShader *shader,
		esShaderType type, const char *fileName) {

	int id;
	switch (type) {
		case ES_SHADER_FRAG :
			id = loadShader(fileName,
					GL_FRAGMENT_SHADER,
					"Fragment shader");
			break;

		default :
		case ES_SHADER_VERT :
			id = loadShader(fileName,
					GL_VERTEX_SHADER,
					"Vertex shader");
			break;
	}

	if (id == 0) {
		esLog(ES_ERRO, "Invalid shader file (%s)\n", fileName);
		return ES_FAIL;
	}

	shader->glShaders[shader->shaderCount++] = id;
	return ES_OK;
}

esErr esShader_compile(esShader *shader) {

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
	return ES_OK;
}

esErr esShader_dual(esShader *shader,
		const char *vertFile, const char *fragFile) {
	esShader_reset(shader);

	if (!esShader_load(shader, ES_SHADER_VERT, vertFile)) return ES_FAIL;
	if (!esShader_load(shader, ES_SHADER_FRAG, fragFile)) return ES_FAIL;
	return esShader_compile(shader);
}

void esShader_use(const esShader *shader) {
	glUseProgram(shader->glProgram);
}

void esShader_free(esShader *shader) {
	glDeleteShader(shader->glProgram);
}

esErr esShader_uniformRegister(esShader *shader,
		esUniform reg, const char *name) {

	int loc = glGetUniformLocation(shader->glProgram, name);
	if (loc < 0) return ES_FAIL;

	shader->uniforms[reg] = loc;
	return ES_OK;
}

esErr esShader_uniformGl(esShader *shader, esUniform reg) {
	return shader->uniforms[reg];
}

