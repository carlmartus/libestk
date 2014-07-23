#include "estk.h"
#include <sys/stat.h>
#include <GL/glew.h>

static void checkError(const char *file, int line) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("OpenGL Error (%s:%d): %d ", file, line, err);

		switch (err) {
#define ERR_CASE_PRINT(cond) case cond : printf("%s\n", #cond); break
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


void *esFileAlloc(const char *file_name) {
	FILE *fd = fopen(file_name, "r");
	if (fd == NULL) return NULL;
	struct stat st;

	stat(file_name, &st);

	char *buf = malloc(st.st_size + 1);
	fread(buf, st.st_size, 1, fd);
	fclose(fd);

	buf[st.st_size] = '\0';

	esLog(ES_INFO, "Loaded %s (%d b)\n%s",
			file_name, (int) st.st_size, buf);
	return buf;
}

