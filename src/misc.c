#include "estk.h"
#include "internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static void checkError(const char *file, int line) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		esLog(ES_ERRO, "OpenGL Error (%s:%d): %d ", file, line, err);

		switch (err) {
#define ERR_CASE_PRINT(cond) case cond : esLog(ES_ERRO, "%s", #cond); break
			ERR_CASE_PRINT(GL_INVALID_ENUM);
			ERR_CASE_PRINT(GL_INVALID_VALUE);
			ERR_CASE_PRINT(GL_INVALID_OPERATION);
			ERR_CASE_PRINT(GL_STACK_OVERFLOW);
			ERR_CASE_PRINT(GL_STACK_UNDERFLOW);
			ERR_CASE_PRINT(GL_OUT_OF_MEMORY);
			default : break;
		}
	}
}
#define check_error() checkError(__FILE__, __LINE__)

void _esCheckGlError(const char *file, int line) {
	checkError(file, line);
}


void *esFileAlloc(const char *fileName) {
	FILE *fd = fopen(fileName, "r");
	if (fd == NULL) return NULL;
	struct stat st;

	stat(fileName, &st);

	char *buf = malloc(st.st_size + 1);
	fread(buf, st.st_size, 1, fd);
	fclose(fd);

	buf[st.st_size] = '\0';

	esLog(ES_INFO, "Loaded file %s (%d b)",
			fileName, (int) st.st_size);
	return buf;
}

