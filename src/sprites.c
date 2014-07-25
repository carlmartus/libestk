#include "estk.h"
#include <stdlib.h>

typedef struct {
	float x, y;
	float u, v;
} SpriteVertex;

static float uv_step;
static int count, renderCount, max;

static esGeoBuf geoBuf;
static esGeo geo;

static SpriteVertex *vertices;

void es2dSpritesInit(int palettDim, int maxSprites) {

	uv_step = 1.0f / (float) palettDim;
	count = 0;
	renderCount = 0;
	max = maxSprites;

	vertices = malloc(sizeof(SpriteVertex)*max*4);

	esGeoBufCreate(&geoBuf);

	esGeoReset(&geo, 2);
	esGeoPoint(&geo, 0, &geoBuf, GEODATA_FLOAT, 2, 0, sizeof(SpriteVertex), ES_FALSE);
	esGeoPoint(&geo, 1, &geoBuf, GEODATA_FLOAT, 2, 8, sizeof(SpriteVertex), ES_TRUE);
}

void es2dSpritesClear(void) {
	free(vertices);
	vertices = NULL;
}

void es2dSpritesPut(float x, float y,
		float radius, int spriteX, int spriteY) {

	float u0 = (float) spriteX * uv_step;
	float v0 = (float) spriteY * uv_step;
	float u1 = u0 + uv_step;
	float v1 = u0 + uv_step;
	SpriteVertex *ptr = vertices + count*6;

	// Triangle 1
	ptr->x = x - radius;
	ptr->y = y - radius;
	ptr->u = u0;
	ptr->v = v0;
	ptr++;

	ptr->x = x + radius;
	ptr->y = y - radius;
	ptr->u = u1;
	ptr->v = v0;
	ptr++;

	ptr->x = x - radius;
	ptr->y = y + radius;
	ptr->u = u0;
	ptr->v = v1;
	ptr++;

	// Triangle 2
	ptr->x = x + radius;
	ptr->y = y + radius;
	ptr->u = u1;
	ptr->v = v1;
	ptr++;

	ptr->x = x - radius;
	ptr->y = y + radius;
	ptr->u = u0;
	ptr->v = v1;
	ptr++;

	ptr->x = x + radius;
	ptr->y = y - radius;
	ptr->u = u1;
	ptr->v = v0;
	ptr++;

	count++;
}

void es2dSpritePrepear(void) {
	esGeoBufArray(&geoBuf, vertices,
			count*6*sizeof(SpriteVertex), GEOBUF_STREAM);
	renderCount = count*6;
	count = 0;
}

void es2dSpritesRender(void) {
	if (renderCount) {
		esGeoRenderArray(&geo, renderCount);
	}
}

