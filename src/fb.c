#include "estk.h"
#include <GL/glew.h>

static const GLenum mipmap_map[] = {
	[TEX_NONE] = GL_NEAREST,
	[TEX_LINEAR] = GL_LINEAR,
};

esErr esFb_create(esFb *fb, int dimension,
		enum esTextureMipmap min, enum esTextureMipmap mag) {
	GLuint glfb, gltex, gldepth;
	glGenFramebuffers(1, &glfb);
	glGenTextures(1, &gltex);

	fb->dimension = dimension;
	fb->gl_fb = glfb;
	fb->gl_tex = gltex;

	// Texture
	glBindTexture(GL_TEXTURE_2D, fb->gl_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimension, dimension,
			0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_map[min]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_map[mag]);
	esCheckGlError();

	// Depth buffer
	glGenRenderbuffers(1, &gldepth);
	glBindRenderbuffer(GL_RENDERBUFFER, gldepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
			dimension, dimension);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	esCheckGlError();

	glBindFramebuffer(GL_FRAMEBUFFER, fb->gl_fb);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gldepth);
	esCheckGlError();
	fb->gl_depth = gldepth;

	// Color channel
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, gltex, 0);
	esCheckGlError();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ES_OK;
}

void esFb_free(esFb *fb) {
	GLuint glfb, gltex, gldepth;
	glfb = fb->gl_fb;
	gltex = fb->gl_tex;
	gldepth = fb->gl_depth;

	glDeleteFramebuffers(1, &glfb);
	glDeleteTextures(1, &gltex);
	glDeleteRenderbuffers(1, &gldepth);
}

void esFb_set(esFb *fb) {
	glViewport(0, 0, fb->dimension, fb->dimension);
	glBindFramebuffer(GL_FRAMEBUFFER, fb->gl_fb);
}

void esFb_unSet(void) {
	int w, h;
	esGame_windowSize(&w, &h);
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void esFb_bind(esFb *fb) {
	glBindTexture(GL_TEXTURE_2D, fb->gl_tex);
}

