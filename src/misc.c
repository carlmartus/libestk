#include "estk.h"
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

