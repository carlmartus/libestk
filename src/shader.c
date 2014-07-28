#include "estk.h"
#include <GL/glew.h>

// Normal shaders
// ==============

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

void esShader_Reset(esShader *shader) {
	shader->shaderCount = 0;
}

esErr esShader_LoadFrag(esShader *shader, const char *fragFile) {
	int idFrag = loadShader(fragFile,
			GL_FRAGMENT_SHADER, "Fragment shader");

	if (idFrag == 0) {
		esLog(ES_ERRO, "Invalid fragment shader file (%s)\n", fragFile);
		return ES_FAIL;
	}

	shader->glShaders[shader->shaderCount++] = idFrag;
	return ES_OK;
}

esErr esShader_LoadVert(esShader *shader, const char *vertFile) {
	int idVert = loadShader(vertFile,
			GL_VERTEX_SHADER, "Vertex shader");

	if (idVert == 0) {
		esLog(ES_ERRO, "Invalid vertex shader file (%s)\n", vertFile);
		return ES_FAIL;
	}

	shader->glShaders[shader->shaderCount++] = idVert;
	return ES_OK;
}

esErr esShader_Compile(esShader *shader) {

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

esErr esShader_Dual(esShader *shader,
		const char *vertFile, const char *fragFile) {
	esShader_Reset(shader);

	if (!esShader_LoadVert(shader, vertFile)) return ES_FAIL;
	if (!esShader_LoadFrag(shader, fragFile)) return ES_FAIL;
	return esShader_Compile(shader);
}

void esShader_Use(const esShader *shader) {
	glUseProgram(shader->glProgram);
}

void esShader_Unload(esShader *shader) {
	glDeleteShader(shader->glProgram);
}

esErr esShader_UniformRegister(esShader *shader,
		esUniform reg, const char *name) {

	int loc = glGetUniformLocation(shader->glProgram, name);
	if (loc < 0) return ES_FAIL;

	shader->uniforms[reg] = loc;
	return ES_OK;
}

esErr esShader_UniformGl(esShader *shader, esUniform reg) {
	return shader->uniforms[reg];
}


// Shader base
// ===========

void esShaderBase_Reset(esShaderBase *sb) {
	sb->shaderCount = 0;
}

esErr esShaderBase_AddVert(esShaderBase *sb, const char *vertFile) {
	GLint id = loadShader(vertFile, GL_VERTEX_SHADER, "Vertex shader");
	sb->glShaders[sb->shaderCount++] = id;
	return id == 0 ? ES_FAIL : ES_OK;
}

esErr esShaderBase_AddFrag(esShaderBase *sb, const char *fragFile) {
	GLint id = loadShader(fragFile, GL_FRAGMENT_SHADER, "Fragment shader");
	sb->glShaders[sb->shaderCount++] = id;
	return id == 0 ? ES_FAIL : ES_OK;
}

esErr esShaderBase_Link(esShaderBase *sb, esShaderBaseType type) {
	switch (type) {
		case ES_SHBASE_COLOR :
			break;

		default : return ES_FAIL;
	}
	return ES_FAIL;
}

