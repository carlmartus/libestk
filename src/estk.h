#ifndef ESTK_H
#define ESTK_H
#include <stdlib.h>
#include <SDL/SDL_mixer.h>

#define ESTK_VERSION "2.2.0"

// Misc {{{

typedef struct { float x, y; } esVec2;
typedef struct { float x, y, z; } esVec3;

enum esBool {
	ES_FALSE = 0,
	ES_TRUE = 1,
};

void _esCheckGlError();
#define esCheckGlError() _esCheckGlError(__FILE__, __LINE__)

void *esFileAlloc(const char *file_name);

// }}}
// Logging {{{

enum {
	ES_INFO, // Information
	ES_WARN, // Warning
	ES_ERRO, // Error
	ES_DEBU, // Debug
};

typedef void (*esLogMessage) (int class, const char *message);

void esLogCallback(esLogMessage callback);
void esLog(int class, const char *fmt, ...);
#define esLogVersion() esLog(ES_DEBU, "libestk Version " ESTK_VERSION)

// }}}
// Game loop {{{

void esGameInit(int screen_width, int screen_height);
void esGameGlSwap(void);
void esGameLoop(void (*frame)(float t), void (*exit)(), int frame_rate);
void esGameLoopQuit(void);
void esGameRegisterKey(int sdlkey, void (*callback)(int key, int down));
void esGameWindowSize(int *w, int *h);

// }}}
// Shader {{{

typedef int esUniform;
#define UNIFORMS_MAX 10
#define SHADERS_MAX 10

typedef struct {
	int glProgram;
	esUniform uniforms[UNIFORMS_MAX];
	int glShaders[SHADERS_MAX];
	int shaderCount;
} esShader;

void esShaderReset(esShader *shader);
int esShaderLoadFrag(esShader *shader, const char *fragFile);
int esShaderLoadVert(esShader *shader, const char *vertFile);
int esShaderCompile(esShader *shader);
int esShaderDual(esShader *shader,
		const char *vertFile, const char *fragFile);
void esShaderUse(const esShader *shader);
void esShaderUnload(esShader *shader);
int esShaderUniformRegister(esShader *shader,
		esUniform reg, const char *name);
int esShaderUniformGl(esShader *shader, esUniform reg);

// }}}
// Geometry {{{

#define GEOBUFS_MAX 8

enum esGeoBufType {
	GEOBUF_STATIC,
	GEOBUF_DYNAMIC,
	GEOBUF_STREAM,
};

typedef struct {
	unsigned int glbuf;
} esGeoBuf;

void esGeoBufCreate(esGeoBuf *buf);
void esGeoBufCopy(esGeoBuf *buf,
		const void *data, size_t size, enum esGeoBufType type);
void esGeoBufDelete(esGeoBuf *buf);

// Geometry
enum esGeoDataType {
	GEODATA_FLOAT,
	GEODATA_INT,
	GEODATA_BYTE,
	GEODATA_UBYTE,
};

typedef struct {
	int bufcount;

	struct {
		esGeoBuf *geobuf;
		enum esGeoDataType datatype;
		int elements;
		size_t offset, stride;
		enum esBool normalized;
	} buf[GEOBUFS_MAX];
} esGeo;

void esGeoReset(esGeo *geo, int bufcount);
void esGeoPoint(esGeo *geo, int id, esGeoBuf *geobuf,
		enum esGeoDataType datatype, int elements,
		size_t offset, size_t stride, enum esBool normalized);
void esGeoRender(const esGeo *geo, int vertices);

// }}}
// Projection {{{

void esProjOrtho(float *mat, float x0, float y0, float x1, float y1);
void esProjPerspective(
		float *mat, float fov, float screenratio, float near, float far,
		esVec3 eye, esVec3 at, esVec3 up);

// }}}
// Texture {{{

enum esTextureMipmap {
	TEX_NONE,
	TEX_LINEAR,
};

typedef struct {
	int w, h;
	int gltexture;
} esTexture;

int esTextureLoad(esTexture *tex, const char *file_name,
		enum esTextureMipmap min, enum esTextureMipmap mag);
void esTextureUse(esTexture *tex);
void esTextureUnload(esTexture *tex);

// }}}
// Font {{{

typedef struct {
	esTexture *texture;
	esShader *shader;

	int vert_count;
	int buf_size, buf_alloc;
	esGeoBuf geo_buf;
	esGeo geo;
	void *buf;
} esFont;

int esFontCreate(esFont *ft, esTexture *tex, esShader *shad,
		int attrib_loc, int attrib_uv, int addition_attribs);
void esFontDelete(esFont *ft);
void esFontAddText(esFont *ft, float offset_x, float offset_y,
		const char *fmt, ...);
void esFontRender(esFont *ft);
void esFontClearBuf(esFont *ft);

// }}}
// Framebuffer {{{

typedef struct {
	int dimension;
	int gl_fb, gl_tex, gl_depth;
} esFrameBuffer;

int esFrameBufferCreate(esFrameBuffer *fb, int dimension,
		enum esTextureMipmap min, enum esTextureMipmap mag);
void esFrameBufferDelete(esFrameBuffer *fb);
void esFrameBufferSet(esFrameBuffer *fb);
void esFrameBufferUnSet(void);
void esFrameBufferBind(esFrameBuffer *fb);

// }}}
// Audio {{{

typedef struct {
	Mix_Chunk *chunk;
} esSound;

int esSoundLoad(esSound *sn, const char *file_name);
void esSoundUnLoad(esSound *sn);
void esSoundPlay(esSound *sn);

typedef struct {
	Mix_Music *music;
} esMusic;

int esMusicLoad(esMusic *mu, const char *file_name);
void esMusicUnLoad(esMusic *mu);
void esMusicPlay(esMusic *mu);
void esMusicHalt(void);

// }}}
// Sprites {{{

void es2dSpritesInit(int palettDim, int maxSprites);
void es2dSpritesClear(void);
void es2dSpritesPut(float x, float y, float radius, int spriteX, int spriteY);
void es2dSpritePrepear(void);
void es2dSpritesRender(void);

// }}}

#endif

