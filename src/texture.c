#include "estk.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

#include "internal.h"

/*
static const GLenum mipmap_map[] = {
	[TEX_NONE] = GL_NEAREST,
	[TEX_LINEAR] = GL_LINEAR,
};*/

static esErr generateTexture(esTexture *tex);


esErr esTexture_load(esTexture *tex, const char *file_name,
		esTextureMipmap min, esTextureMipmap mag) {
	SDL_Surface *surf = IMG_Load(file_name);
	if (surf == NULL) return ES_FAIL;

	tex->w = surf->w;
	tex->h = surf->h;

	GLuint gltex;
	glGenTextures(1, &gltex);
	tex->gltexture = gltex;
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);

	int mode = surf->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h,
			0, mode, GL_UNSIGNED_BYTE, surf->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_map[min]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_map[mag]);

	SDL_FreeSurface(surf);
	return ES_OK;
}

esErr esTexture_createColor(esTexture *tex,
		unsigned width, unsigned height,
		esTextureMipmap min, esTextureMipmap mag) {

	if (generateTexture(tex) != ES_OK) return ES_FAIL;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_map[min]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_map[mag]);
}

void esTexture_use(esTexture *tex) {
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);
}

void esTexture_useWithId(unsigned textureNumber, esTexture *tex) {
	glActiveTexture(textureNumber);
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);
}

void esTexture_free(esTexture *tex) {
	GLuint gltex = tex->gltexture;
	glDeleteTextures(1, &gltex);
}


static esErr generateTexture(esTexture *tex) {
	GLuint gltex;
	glGenTextures(1, &gltex);
	tex->gltexture = gltex;
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);
	return ES_OK;
}

