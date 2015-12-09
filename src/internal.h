#pragma once

static const GLenum map_mipmap[] = {
	[ES_TEX_NONE] = GL_NEAREST,
	[ES_TEX_LINEAR] = GL_LINEAR,
};

static const GLenum map_channelCount[] = {
	[1] = GL_RED,
	[2] = GL_RG,
	[3] = GL_RGB,
	[4] = GL_RGBA,
};

static const unsigned int map_type[] = {
	[ES_DATA_FLOAT] = GL_FLOAT,
	[ES_DATA_INT] = GL_INT,
	[ES_DATA_UINT] = GL_UNSIGNED_INT,
	[ES_DATA_SHORT] = GL_SHORT,
	[ES_DATA_USHORT] = GL_UNSIGNED_SHORT,
	[ES_DATA_BYTE] = GL_BYTE,
	[ES_DATA_UBYTE] = GL_UNSIGNED_BYTE,
};

static const unsigned int map_draw[] = {
	[ES_GEOBUF_STATIC] = GL_STATIC_DRAW,
	[ES_GEOBUF_DYNAMIC] = GL_DYNAMIC_DRAW,
	[ES_GEOBUF_STREAM] = GL_STREAM_DRAW,
};

