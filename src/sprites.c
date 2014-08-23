#include "estk.h"
#include <math.h>
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

void esSprites2d_init(int palettDim, int maxSprites) {

	uv_step = 1.0f / (float) palettDim;
	count = 0;
	renderCount = 0;
	max = maxSprites;

	vertices = malloc(sizeof(SpriteVertex)*max*4);

	esGeoBuf_create(&geoBuf);

	esGeo_reset(&geo, 2);
	esGeo_point(&geo, 0, &geoBuf, GEODATA_FLOAT, 2, 0, sizeof(SpriteVertex), ES_FALSE);
	esGeo_point(&geo, 1, &geoBuf, GEODATA_FLOAT, 2, 8, sizeof(SpriteVertex), ES_TRUE);
}

void esSprites2d_clear(void) {
	free(vertices);
	vertices = NULL;
}

void esSprites2d_put(float x, float y, float radius,
		float rotate, int spriteX, int spriteY, unsigned flags) {

	if (count >= max) {
		esLog(ES_WARN, "Sprite count reached");
		return;
	}

	float u0 = (float) spriteX * uv_step;
	float v0 = (float) spriteY * uv_step;
	float u1 = u0 + uv_step;
	float v1 = v0 + uv_step;

	if (flags & ES_SPRITE_FLIPX) {
		float swap = u0;
		u0 = u1;
		u1 = swap;
	}

	if (flags & ES_SPRITE_FLIPY) {
		float swap = v0;
		v0 = v1;
		v1 = swap;
	}

	//float bx = sinf(rotate)*M_SQRT2*radius;
	//float by = cosf(rotate)*M_SQRT2*radius;
	float cr = cosf(rotate);
	float sr = sinf(rotate);
	float bx = (cr - sr) * radius;
	float by = (sr + cr) * radius;

	SpriteVertex *ptr = vertices + count*6;

	// Triangle 1
	ptr->x = x - by;
	ptr->y = y + bx;
	ptr->u = u0;
	ptr->v = v0;
	ptr++;

	ptr->x = x + bx;
	ptr->y = y + by;
	ptr->u = u1;
	ptr->v = v0;
	ptr++;

	ptr->x = x - bx;
	ptr->y = y - by;
	ptr->u = u0;
	ptr->v = v1;
	ptr++;

	count++;

	// Triangle 2
	ptr->x = x + by;
	ptr->y = y - bx;
	ptr->u = u1;
	ptr->v = v1;
	ptr++;

	ptr->x = x - bx;
	ptr->y = y - by;
	ptr->u = u0;
	ptr->v = v1;
	ptr++;

	ptr->x = x + bx;
	ptr->y = y + by;
	ptr->u = u1;
	ptr->v = v0;
	ptr++;

	count++;
}

void esSprites2d_prepear(void) {
	esGeoBuf_array(&geoBuf, vertices,
			count*6*sizeof(SpriteVertex), GEOBUF_STREAM);
	renderCount = count*6;
	//renderCount = count*3;
	count = 0;
}

void esSprites2d_render(void) {
	if (renderCount) {
		esGeo_renderArray(&geo, renderCount);
	}
}

