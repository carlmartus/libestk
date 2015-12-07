#include "estk.h"
#include <GL/glew.h>

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

esErr esMultiRender_create(esMultiRender *mr,
		unsigned width, unsigned height, unsigned textureCount) {

	mr->textureCount = textureCount;

	glGenFramebuffers(1, &mr->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mr->frameBuffer);

	// Create textures
	glGenTextures(textureCount, mr->renderTextures);

	for (int i=0; i<textureCount; i++) {
		glBindTexture(GL_TEXTURE_2D, mr->renderTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	// Bind textures
	static const GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
	};
	glDrawBuffers(textureCount, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		esMultiRender_destroy(mr);
		return ES_FAIL;
	}

	return ES_OK;
}

void esMultiRender_destroy(esMultiRender *mr) {
	// TODO Delete everything
}

