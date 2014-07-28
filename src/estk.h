#ifndef ESTK_H
#define ESTK_H
#include <stdlib.h>
#include <SDL/SDL_mixer.h>

#define ESTK_VERSION "2.2.1"

// Misc {{{

typedef enum {
	ES_FAIL=0,
	ES_OK=1,
} esErr;

typedef struct { float x, y; } esVec2;
typedef struct { float x, y, z; } esVec3;

enum esBool {
	ES_FALSE = 0,
	ES_TRUE = 1,
};

void _esCheckGlError();
#define esCheckGlError() _esCheckGlError(__FILE__, __LINE__)
//#define esCheckGlError()

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
#define esLogUnimplemented() esLog(ES_DEBU, "Unimplemented %s:%d", __FILE__, __LINE__)

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

// Normal shaders
typedef int esUniform;
#define UNIFORMS_MAX 10
#define SHADERS_MAX 10

typedef enum {
	ES_SHADER_VERT,
	ES_SHADER_FRAG,
} esShaderType;

typedef struct {
	int glProgram;
	esUniform uniforms[UNIFORMS_MAX];
	int glShaders[SHADERS_MAX];
	int shaderCount;
} esShader;

void esShader_Reset(esShader *shader);
esErr esShader_LoadFrag(esShader *shader, const char *fragFile);
esErr esShader_LoadVert(esShader *shader, const char *vertFile);
esErr esShader_Compile(esShader *shader);
esErr esShader_Dual(esShader *shader,
		const char *vertFile, const char *fragFile);
void esShader_Use(const esShader *shader);
void esShader_Unload(esShader *shader);
esErr esShader_UniformRegister(esShader *shader,
		esUniform reg, const char *name);
esErr esShader_UniformGl(esShader *shader, esUniform reg);

// Shader base abstraction
typedef struct {
	float x, y, z;
	uint8_t r, g, b, a;
} esShaderBaseColor;

typedef enum {
	ES_SHBASE_COLOR,
} esShaderBaseType;

typedef struct {
	esShaderBaseType type;
	int uniMask;
	esShader sh;
} esShaderBase;

esErr esShaderBase_Reset(esShaderBase *sb, esShaderBaseType type);
esErr esShaderBase_AddVert(esShaderBase *sb,
		esShaderType type, const char *vertFile);
esErr esShaderBase_Link(esShaderBase *sb);
esErr esShaderBase_Camera(esShaderBase *sb, float *mat);

// }}}
// Geometry {{{

#define GEOBUFS_MAX 8

typedef enum {
	GEOBUF_STATIC,
	GEOBUF_DYNAMIC,
	GEOBUF_STREAM,
} esGeoBufType;

typedef enum {
	GEODATA_FLOAT,
	GEODATA_INT,
	GEODATA_UINT,
	GEODATA_SHORT,
	GEODATA_USHORT,
	GEODATA_BYTE,
	GEODATA_UBYTE,
} esGeoDataType;

typedef struct {
	esGeoDataType dataType;
	unsigned int glBuf;
} esGeoBuf;

void esGeoBufCreate(esGeoBuf *buf);
void esGeoBufArray(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type);
void esGeoBufElement(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type);
void esGeoBufDelete(esGeoBuf *buf);

typedef struct {
	int bufCount;

	struct {
		esGeoBuf *geobuf;
		esGeoDataType datatype;
		int elements;
		size_t offset, stride;
		enum esBool normalized;
	} buf[GEOBUFS_MAX];
} esGeo;

void esGeoReset(esGeo *geo, int bufCount);
void esGeoPoint(esGeo *geo, int id, esGeoBuf *geobuf,
		esGeoDataType datatype, int elements,
		size_t offset, size_t stride, enum esBool normalized);
void esGeoRenderArray(const esGeo *geo, int vertices);
void esGeoRenderElements(const esGeo *geo, const esGeoBuf *indices,
		esGeoDataType dataType, int vertexCount);

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

esErr esTextureLoad(esTexture *tex, const char *file_name,
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

esErr esFontCreate(esFont *ft, esTexture *tex, esShader *shad,
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

esErr esFrameBufferCreate(esFrameBuffer *fb, int dimension,
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

esErr esSoundLoad(esSound *sn, const char *file_name);
void esSoundUnLoad(esSound *sn);
void esSoundPlay(esSound *sn);

typedef struct {
	Mix_Music *music;
} esMusic;

esErr esMusicLoad(esMusic *mu, const char *file_name);
void esMusicUnLoad(esMusic *mu);
void esMusicPlay(esMusic *mu);
void esMusicHalt(void);

// }}}
// Sprites {{{

#define ES_SPRITE_FLIPX 1
#define ES_SPRITE_FLIPY 2
#define ES_SPRITE_FLIPXY (ES_SPRITE_FLIPX | ES_SPRITE_FLIPY)

void es2dSpritesInit(int palettDim, int maxSprites);
void es2dSpritesClear(void);
void es2dSpritesPut(float x, float y, float radius,
		float rotate, int spriteX, int spriteY, unsigned flags);
void es2dSpritePrepear(void);
void es2dSpritesRender(void);

// }}}

#endif

