#include "estk.h"
#include <GL/glew.h>

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

esErr esMultiRender_create(esMultiRender *mr,
		unsigned width, unsigned height, unsigned textureCount, ...) {

	mr->textureCount = textureCount;
	mr->width = width;
	mr->height = height;

	// Create textures
	glGenTextures(textureCount, mr->renderTextures);

	static const GLenum channelCountMap[] = {
		[1] = GL_RED,
		[2] = GL_RG,
		[3] = GL_RGB,
		[4] = GL_RGBA,
	};

	va_list args;
	va_start(args, textureCount);

	for (int i=0; i<textureCount; i++) {
		int channelCount = va_arg(args, int);
		if (channelCount < 1 || channelCount > 4) {
			glDeleteTextures(textureCount, mr->renderTextures);
			return ES_FAIL;
		}

		glBindTexture(GL_TEXTURE_2D, mr->renderTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
				channelCountMap[channelCount], GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	va_end(args);

	// Generate frame buffer
	glGenFramebuffers(1, &mr->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mr->frameBuffer);

	// Depth buffer
	glGenRenderbuffers(1, &mr->depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mr->depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, mr->depthBuffer);

	// Bind textures
	static const GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
	};

	for (int i=0; i<textureCount; i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i,
				GL_TEXTURE_2D, mr->renderTextures[i], 0);
	}

	glDrawBuffers(textureCount, drawBuffers);

	// Validate
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		esMultiRender_unBind();
		esMultiRender_destroy(mr);
		return ES_FAIL;
	}

	return ES_OK;
}

void esMultiRender_destroy(esMultiRender *mr) {
	// TODO Delete everything
}

void esMultiRender_bind(esMultiRender *mr) {
	glBindFramebuffer(GL_FRAMEBUFFER, mr->frameBuffer);
	glViewport(0, 0, mr->width, mr->height);
}

void esMultiRender_unBind(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
