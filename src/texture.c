#include "estk.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

static const GLenum mipmap_map[] = {
	[TEX_NONE] = GL_NEAREST,
	[TEX_LINEAR] = GL_LINEAR,
};

int esTextureLoad(esTexture *tex, const char *file_name,
		enum esTextureMipmap min, enum esTextureMipmap mag) {
	SDL_Surface *surf = IMG_Load(file_name);
	if (surf == NULL) return 1;

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
	return 0;
}

void esTextureUse(esTexture *tex) {
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);
}

void esTextureUnload(esTexture *tex) {
	GLuint gltex = tex->gltexture;
	glDeleteTextures(1, &gltex);
}

