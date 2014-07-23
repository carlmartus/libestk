#include "estk.h"

static float uv_step;

void es2dSpritesInit(esTexture *sprites, int palettDim, int maxSprites) {
	uv_step = 1.0f / (float) palettDim;
}

void es2dSpritesClear(void) {
}

void es2dSpritesPut(esVec2 loc, int spriteX, int spriteY) {
}


