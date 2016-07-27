#include "estk.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "internal.h"

static esErr generateTexture(esTexture *tex);

esErr esTexture_createRaw(esTexture *tex, unsigned channelCount,
		unsigned width, unsigned height, esDataType dataType, const void *ptr,
		esTextureMipmap mipMapMin, esTextureMipmap mipMapMag) {

	tex->w = width;
	tex->h = height;

	GLuint gltex;
	glGenTextures(1, &gltex);
	tex->gltexture = gltex;
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);

	if (channelCount < 1 || channelCount > 4) return ES_FAIL;

	int mode = map_channelCount[channelCount];

	glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height,
			0, mode, map_type[dataType], ptr);
	esCheckGlError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			map_mipmap[mipMapMin]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			map_mipmap[mipMapMag]);

	esCheckGlError();
	return ES_OK;
}

#ifdef ES_OPT_IMAGE
esErr esTexture_load(esTexture *tex, const char *file_name,
		esTextureMipmap mipMapMin, esTextureMipmap mipMapMag) {
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
	esCheckGlError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			map_mipmap[mipMapMin]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			map_mipmap[mipMapMag]);
	esCheckGlError();

	SDL_FreeSurface(surf);
	return ES_OK;
}
#endif

esErr esTexture_createColor(esTexture *tex,
		unsigned width, unsigned height,
		esTextureMipmap mipMapMin, esTextureMipmap mipMapMag) {

	if (generateTexture(tex) != ES_OK) return ES_FAIL;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			map_mipmap[mipMapMin]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			map_mipmap[mipMapMag]);

	esCheckGlError();
	return ES_OK;
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

