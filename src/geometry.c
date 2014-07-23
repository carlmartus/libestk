#include "estk.h"
#include <GL/glew.h>

void esGeoBufCreate(esGeoBuf *buf) {
	glGenBuffers(1, &buf->glbuf);
	esCheckGlError();
}

void esGeoBufCopy(esGeoBuf *buf,
		const void *data, size_t size, enum esGeoBufType type) {
	static const unsigned int map[] = {
		[GEOBUF_STATIC] = GL_STATIC_DRAW,
		[GEOBUF_DYNAMIC] = GL_DYNAMIC_DRAW,
		[GEOBUF_STREAM] = GL_STREAM_DRAW,
	};

	glBindBuffer(GL_ARRAY_BUFFER, buf->glbuf);
	esCheckGlError();

	glBufferData(GL_ARRAY_BUFFER, size, data, map[type]);
	esCheckGlError();
}

void esGeoBufDelete(esGeoBuf *buf) {
	glDeleteBuffers(1, &buf->glbuf);
	esCheckGlError();
}

void esGeoReset(esGeo *geo, int bufcount) {
	geo->bufcount = bufcount;
}

void esGeoPoint(esGeo *geo, int id, esGeoBuf *geobuf,
		enum esGeoDataType datatype, int elements,
		size_t offset, size_t stride, enum esBool normalized)
{
	geo->buf[id].geobuf = geobuf;
	geo->buf[id].datatype = datatype;
	geo->buf[id].elements = elements;
	geo->buf[id].offset = offset;
	geo->buf[id].stride = stride;
	geo->buf[id].normalized = normalized;
}

void esGeoRender(const esGeo *geo, int vertices) {
	static const unsigned int map[] = {
		[GEODATA_FLOAT] = GL_FLOAT,
		[GEODATA_INT] = GL_INT,
		[GEODATA_BYTE] = GL_BYTE,
		[GEODATA_UBYTE] = GL_UNSIGNED_BYTE,
	};

	int bufcount = geo->bufcount;
	const esGeoBuf *last = NULL;

	int i;
	for (i=0; i<bufcount; i++) {
		glEnableVertexAttribArray(i);

		if (last != geo->buf[i].geobuf) {
			last = geo->buf[i].geobuf;
			glBindBuffer(GL_ARRAY_BUFFER, last->glbuf);
		}

		glVertexAttribPointer(i,
				geo->buf[i].elements,
				map[geo->buf[i].datatype],
				geo->buf[i].normalized == ES_TRUE ? GL_TRUE : GL_FALSE,
				geo->buf[i].stride,
				(void*) geo->buf[i].offset);
		esCheckGlError();
	}

	glDrawArrays(GL_TRIANGLES, 0, vertices);
	esCheckGlError();

	for (i=0; i<bufcount; i++) glDisableVertexAttribArray(i);
	esCheckGlError();
}

