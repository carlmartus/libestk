#include "estk.h"
#include "internal.h"

//==============================================================================
// Interal shader operations
//==============================================================================

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

static int loadShaderText(const char *source,
		GLenum shaderType, const char *shaderInfo) {
	esCheckGlError();

	int shad = glCreateShader(shaderType);
	glShaderSource(shad, 1, (const char**) &source, NULL);
	glCompileShader(shad);

	if (!checkShader(shad, shaderInfo)) {
		esCheckGlError();
		return 0;
	}
	esCheckGlError();

	//esLog(ES_INFO, "Loaded shader %s", fileName);
	esCheckGlError();
	return shad;
}


//==============================================================================
// API shader operations
//==============================================================================

void esShader_reset(esShader *shader) {
	shader->shaderCount = 0;
}

esErr esShader_loadFile(esShader *shader,
		esShaderType type, const char *fileName) {

	char *content = esFileAlloc(fileName);
	if (content == NULL) {
		return ES_FAIL;
	}

	int ret = esShader_loadText(shader, type, content);
	free(content);

	return ret;
}

esErr esShader_loadText(esShader *shader,
		esShaderType type, const char *source) {
	int id;
	switch (type) {
		case ES_SHADER_FRAG :
			id = loadShaderText(source,
					GL_FRAGMENT_SHADER,
					"Fragment shader");
			break;

		default :
		case ES_SHADER_VERT :
			id = loadShaderText(source,
					GL_VERTEX_SHADER,
					"Vertex shader");
			break;
	}

	if (id == 0) {
		esLog(ES_ERRO, "Invalid shader source (%s)\n", source);
		return ES_FAIL;
	}

	shader->glShaders[shader->shaderCount++] = id;
	return ES_OK;
}

esErr esShader_compile(esShader *shader,
		const esShaderAttrib *attribs, int attribCount) {

	int program = glCreateProgram();
	shader->glProgram = program;

	int i;
	for (i=0; i<shader->shaderCount; i++) {
		glAttachShader(program, shader->glShaders[i]);
		esCheckGlError();
	}

	for (i=0; i<attribCount; i++) {
		glBindAttribLocation(shader->glProgram,
				attribs[i].location, attribs[i].name);
		esCheckGlError();
	}

	glLinkProgram(program);
	esCheckGlError();

	for (i=0; i<shader->shaderCount; i++) {
		glDeleteShader(shader->glShaders[i]);
		esCheckGlError();
	}

	return ES_OK;
}

esErr esShader_dualFile(esShader *shader,
		const char *vertFile, const char *fragFile,
		const esShaderAttrib *attribs, int attribCount) {

	esShader_reset(shader);

	if (!esShader_loadFile(shader, ES_SHADER_VERT, vertFile)) return ES_FAIL;
	if (!esShader_loadFile(shader, ES_SHADER_FRAG, fragFile)) return ES_FAIL;
	return esShader_compile(shader, attribs, attribCount);
}

esErr esShader_dualText(esShader *shader,
		const char *vertSource, const char *fragSource,
		const esShaderAttrib *attribs, int attribCount) {

	esShader_reset(shader);

	if (!esShader_loadText(shader, ES_SHADER_VERT, vertSource)) return ES_FAIL;
	if (!esShader_loadText(shader, ES_SHADER_FRAG, fragSource)) return ES_FAIL;
	return esShader_compile(shader, attribs, attribCount);
}

void esShader_use(const esShader *shader) {
	glUseProgram(shader->glProgram);
}

void esShader_free(esShader *shader) {
	glDeleteProgram(shader->glProgram);
	esCheckGlError();
}

esErr esShader_uniformRegister(esShader *shader,
		esUniform reg, const char *name) {

	int loc = glGetUniformLocation(shader->glProgram, name);
	if (loc < 0) return ES_FAIL;

	shader->uniforms[reg] = loc;
	return ES_OK;
}

esGLuint esShader_uniformGl(esShader *shader, esUniform reg) {
	return shader->uniforms[reg];
}

