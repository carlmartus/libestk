#include "estk.h"
#include "internal.h"

esErr esMultiRender_create(esMultiRender *mr,
		unsigned width, unsigned height,
		esTextureMipmap mipMapMin, esTextureMipmap mipMapMag,
		unsigned textureCount, ...) {

	mr->textureCount = textureCount;
	mr->width = width;
	mr->height = height;

	// Create textures
	glGenTextures(textureCount, mr->renderTextures);

	va_list args;
	va_start(args, textureCount);

	for (int i=0; i<textureCount; i++) {
		int channelCount = va_arg(args, int);
		if (channelCount < 1 || channelCount > 4) {
			esLog(ES_ERRO,
					"Bad texture channel count (%d) for texture ID %d",
					channelCount, i);
			glDeleteTextures(textureCount, mr->renderTextures);
			return ES_FAIL;
		}

		glBindTexture(GL_TEXTURE_2D, mr->renderTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, map_channelCount[channelCount],
				width, height, 0, map_channelCount[channelCount],
				GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				map_mipmap[mipMapMag]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				map_mipmap[mipMapMin]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

#ifdef ES_OPT_GL_GLEW
	glDrawBuffers(textureCount, drawBuffers);
#endif

#ifdef ES_OPT_GL_GLEW
	glDrawBuffers(textureCount, drawBuffers);
#endif

	// Validate
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		esMultiRender_unBind();
		esMultiRender_destroy(mr);
		return ES_FAIL;
	}

	return ES_OK;
}

void esMultiRender_destroy(esMultiRender *mr) {
	glDeleteTextures(mr->textureCount, mr->renderTextures);
	glDeleteRenderbuffers(1, &mr->depthBuffer);
	glDeleteFramebuffers(1, &mr->frameBuffer);
}

void esMultiRender_bind(esMultiRender *mr) {
	glBindFramebuffer(GL_FRAMEBUFFER, mr->frameBuffer);
	glViewport(0, 0, mr->width, mr->height);
}

void esMultiRender_unBind(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void esMultiRender_bindTexture(esMultiRender *mr, unsigned textureId) {
	glBindTexture(GL_TEXTURE_2D, mr->renderTextures[textureId]);
}

void esMultiRender_bindTextureWithId(esMultiRender *mr, unsigned textureId,
		unsigned openGlTextureId) {
	glActiveTexture(GL_TEXTURE0 + openGlTextureId);
	glBindTexture(GL_TEXTURE_2D, mr->renderTextures[textureId]);
}

void esMultiRender_bindAllTextures(esMultiRender *mr) {
	for (int i=0; i<mr->textureCount; i++) {
		esMultiRender_bindTextureWithId(mr, i, i);
	}

	esCheckGlError();
}

