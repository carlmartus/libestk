#include "estk.h"
#include <GL/glew.h>

static const unsigned int map_type[] = {
	[GEODATA_FLOAT] = GL_FLOAT,
	[GEODATA_INT] = GL_INT,
	[GEODATA_UINT] = GL_UNSIGNED_INT,
	[GEODATA_SHORT] = GL_SHORT,
	[GEODATA_USHORT] = GL_UNSIGNED_SHORT,
	[GEODATA_BYTE] = GL_BYTE,
	[GEODATA_UBYTE] = GL_UNSIGNED_BYTE,
};

static const unsigned int map_draw[] = {
	[GEOBUF_STATIC] = GL_STATIC_DRAW,
	[GEOBUF_DYNAMIC] = GL_DYNAMIC_DRAW,
	[GEOBUF_STREAM] = GL_STREAM_DRAW,
};

void esGeoBuf_create(esGeoBuf *buf) {
	glGenBuffers(1, &buf->glBuf);
	esCheckGlError();
}

void esGeoBuf_array(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type) {

	glBindBuffer(GL_ARRAY_BUFFER, buf->glBuf);
	esCheckGlError();

	glBufferData(GL_ARRAY_BUFFER, size, data, map_draw[type]);
	esCheckGlError();
}

void esGeoBuf_arraySub(esGeoBuf *buf,
		const void *data, size_t size, int bufOffset) {

	glBindBuffer(GL_ARRAY_BUFFER, buf->glBuf);
	esCheckGlError();

	glBufferSubData(GL_ARRAY_BUFFER, bufOffset, size, data);
	esCheckGlError();
}

void esGeoBuf_element(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type) {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->glBuf);
	esCheckGlError();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, map_draw[type]);
	esCheckGlError();
}

void esGeoBuf_free(esGeoBuf *buf) {
	glDeleteBuffers(1, &buf->glBuf);
	esCheckGlError();
}

void esGeo_reset(esGeo *geo, int bufCount) {
	geo->bufCount = bufCount;
}

void esGeo_point(esGeo *geo, int id, esGeoBuf *geobuf,
		esGeoDataType datatype, int elements,
		size_t offset, size_t stride, esBool normalized)
{
	geo->buf[id].geobuf = geobuf;
	geo->buf[id].datatype = datatype;
	geo->buf[id].elements = elements;
	geo->buf[id].offset = offset;
	geo->buf[id].stride = stride;
	geo->buf[id].normalized = normalized;
}

static void preRender(const esGeo *geo) {

	int i, bufCount = geo->bufCount;
	const esGeoBuf *last = NULL;

	for (i=0; i<bufCount; i++) {
		glEnableVertexAttribArray(i);

		if (last != geo->buf[i].geobuf) {
			last = geo->buf[i].geobuf;
			glBindBuffer(GL_ARRAY_BUFFER, last->glBuf);
		}

		glVertexAttribPointer(i,
				geo->buf[i].elements,
				map_type[geo->buf[i].datatype],
				geo->buf[i].normalized == ES_TRUE ? GL_TRUE : GL_FALSE,
				geo->buf[i].stride,
				(void*) geo->buf[i].offset);
		esCheckGlError();
	}
}

static void postRender(const esGeo *geo) {
	int i, bufCount = geo->bufCount;
	for (i=0; i<bufCount; i++) glDisableVertexAttribArray(i);
	esCheckGlError();
}

void esGeo_renderArray(const esGeo *geo, int vertices) {
	preRender(geo);
	glDrawArrays(GL_TRIANGLES, 0, vertices);
	esCheckGlError();
	postRender(geo);
}

void esGeo_renderElements(const esGeo *geo, const esGeoBuf *indices,
		esGeoDataType dataType, int vertexCount) {

	preRender(geo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->glBuf);
	esCheckGlError();

	glDrawElements(GL_TRIANGLES, vertexCount,
			map_type[dataType], (void*) 0);
	esCheckGlError();

	postRender(geo);
}

