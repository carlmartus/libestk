#include "estk.h"
#include <GL/glew.h>

static int check_shader(GLuint id, const char *shader_info) {
	GLint result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		char info_buf[500];
		glGetShaderInfoLog(id, sizeof(info_buf)-1, NULL, info_buf);
		printf("Shader info (%s):\n%s\n", shader_info, info_buf);
		esCheckGlError();
		return 1;
	}

	return 0;
}

int shader_load(const char *file_name,
		GLenum shader_type, const char *shader_info) {

	char *content = esFileAlloc(file_name);
	if (content == NULL) {
		return 0;
	}

	int shad = glCreateShader(shader_type);
	glShaderSource(shad, 1, (const char**) &content , NULL);
	glCompileShader(shad);
	esCheckGlError();
	free(content);

	if (check_shader(shad, shader_info)) return 0;

	return shad;
}

int esShaderLoad(esShader *shader,
		const char *vertFile, const char *fragFile) {
	int idvert = shader_load(vertFile, GL_VERTEX_SHADER, "Vertex shader");
	if (idvert == 0) {
		printf("Invalid vertex shader file (%s)\n", vertFile);
		return 1;
	}

	int idfrag = shader_load(fragFile, GL_FRAGMENT_SHADER, "Fragment shader");
	if (idfrag == 0) {
		printf("Invalid fragment shader file (%s)\n", fragFile);
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

