#include "estk.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <GL/glew.h>

#define MAX_STRING 512
#define FONT_INIT 4096

struct font_vert {
	float x, y;
	float u, v;
};

esErr esFont_create(esFont *ft, esTexture *tex, esShader *shad,
		int attrib_loc, int attrib_uv, int addition_attribs) {
	ft->texture = tex;
	ft->shader = shad;

	ft->vert_count = 0;

	ft->buf = malloc(FONT_INIT*sizeof(struct font_vert)*6);
	ft->buf_size = 0;
	ft->buf_alloc = FONT_INIT;

	esGeoBuf_create(&ft->geo_buf);

	esGeo_reset(&ft->geo, 2 + addition_attribs);
	esGeo_point(&ft->geo, 0, &ft->geo_buf, GEODATA_FLOAT, 2, 0,
			sizeof(struct font_vert), ES_FALSE);
	esGeo_point(&ft->geo, 1, &ft->geo_buf, GEODATA_FLOAT, 2,
			offsetof(struct font_vert, u),
			sizeof(struct font_vert), ES_TRUE);

	return ES_OK;
}

static void buf_check(esFont *ft, int add) {
	int required = ft->buf_size + add;

	if (required < ft->buf_alloc) return;

	while (ft->buf_alloc < required) {
		ft->buf_alloc <<= 1;
	}
	ft->buf = realloc(ft->buf, ft->buf_alloc);
}

void esFont_addText(esFont *ft,
		float offset_x, float offset_y, const char *fmt, ...) {
	va_list args;
	char buf[MAX_STRING];

	va_start(args, fmt);
	vsnprintf(buf, MAX_STRING, fmt, args);
	va_end(args);

	int len = strlen(buf);
	int size = sizeof(struct font_vert)*6*len;
	buf_check(ft, size);

	struct font_vert *itr = ft->buf + ft->buf_size;
	int i;
	for (i=0; i<len; i++) {

		char ch = buf[i];
		int x = ch & 0xf;
		int y = ch >> 4;

#define FONT_UV_INC (1.0f / 16.0f)

		float u0 = FONT_UV_INC * (float) x;
		float v0 = FONT_UV_INC * (float) y;
		float u1 = u0 + FONT_UV_INC;
		float v1 = v0 + FONT_UV_INC;

#define FONT_PUSH(a, b, c, d)\
		itr->x = offset_x + a;\
		itr->y = offset_y + b;\
		itr->u = c;\
		itr->v = d;\
		itr++;

		FONT_PUSH(0.0f, 0.0f, u0, v0);
		FONT_PUSH(1.0f, 0.0f, u1, v0);
		FONT_PUSH(0.0f, 1.0f, u0, v1);

		FONT_PUSH(1.0f, 0.0f, u1, v0);
		FONT_PUSH(1.0f, 1.0f, u1, v1);
		FONT_PUSH(0.0f, 1.0f, u0, v1);

		ft->vert_count += 6;
		offset_x += 1.0f;
	}

	ft->buf_size += size;
}

void esFont_delete(esFont *ft) {
	free(ft->buf);
	ft->buf = NULL;

	esGeoBuf_free(&ft->geo_buf);
}

void esFont_render(esFont *ft) {
	if (ft->shader) {
		esShader_use(ft->shader);
	}

	if (ft->vert_count > 0) {

		esGeoBuf_array(&ft->geo_buf, ft->buf, ft->buf_size, GEOBUF_STREAM);
		esGeo_renderArray(&ft->geo, ft->vert_count);
	}
}

void esFont_clearBuf(esFont *ft) {
	ft->vert_count = 0;
}

