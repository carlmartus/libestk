#ifndef ESTK_H
#define ESTK_H
#include <stdlib.h>
#include <SDL/SDL_mixer.h>

#define ESTK_VERSION "3.1.0"

// Misc {{{

typedef enum {
	ES_FAIL=0,
	ES_OK=1,
} esErr;

typedef enum {
	ES_FALSE = 0,
	ES_TRUE = 1,
} esBool;

void _esCheckGlError();
#define esCheckGlError() _esCheckGlError(__FILE__, __LINE__)
//#define esCheckGlError()

void *esFileAlloc(const char *file_name);

// }}}
// Math {{{

// Vectors
typedef struct { float x, y; } esVec2f;
typedef struct { float x, y, z; } esVec3f;

// Matrices
typedef struct {
	float mat[3*3];
} esMat3f;

typedef struct {
	float mat[4*4];
} esMat4f;

esVec3f esMat3f_MulVec3f(const esMat3f *m, esVec3f v);

// Quaternions
typedef struct {
	float x, y, z, w;
} esQuatf;

esQuatf esQuatf_normalize(esQuatf q);
esQuatf esQuatf_mul(esQuatf q0, esQuatf q1);
esQuatf esQuatf_local(esQuatf q, float angle);
void esQuatf_matrix(esMat3f *dst, esQuatf q);

// }}}
// Logging {{{

typedef enum {
	ES_INFO, // Information
	ES_WARN, // Warning
	ES_ERRO, // Error
	ES_DEBU, // Debug
} esLogClass;

typedef void (*esLogMessage) (esLogClass class, const char *message);

void esLogCallback(esLogMessage callback);
void esLog(int class, const char *fmt, ...);
#define esLogVersion() esLog(ES_DEBU, "libestk Version " ESTK_VERSION)
#define esLogUnimplemented() esLog(ES_DEBU, "Unimplemented %s:%d", __FILE__, __LINE__)

// }}}
// Game loop {{{

void esGame_init(int screen_width, int screen_height);
void esGame_glSwap(void);
void esGame_loop(void (*frame)(float t), void (*exit)(), int frame_rate);
void esGame_loopQuit(void);
void esGame_registerKey(int sdlkey, void (*callback)(int key, int down));
void esGame_windowSize(int *w, int *h);

// }}}
// Shader {{{

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

void esShader_reset(esShader *shader);
esErr esShader_load(esShader *shader,
		esShaderType type, const char *fileName);
esErr esShader_compile(esShader *shader);
esErr esShader_dual(esShader *shader,
		const char *vertFile, const char *fragFile);
void esShader_use(const esShader *shader);
void esShader_free(esShader *shader);
esErr esShader_uniformRegister(esShader *shader,
		esUniform reg, const char *name);
esErr esShader_uniformGl(esShader *shader, esUniform reg);

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

void esGeoBuf_create(esGeoBuf *buf);
void esGeoBuf_array(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type);
void esGeoBuf_element(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type);
void esGeoBuf_free(esGeoBuf *buf);

typedef struct {
	int bufCount;

	struct {
		esGeoBuf *geobuf;
		esGeoDataType datatype;
		int elements;
		size_t offset, stride;
		esBool normalized;
	} buf[GEOBUFS_MAX];
} esGeo;

void esGeo_reset(esGeo *geo, int bufCount);
void esGeo_point(esGeo *geo, int id, esGeoBuf *geobuf,
		esGeoDataType datatype, int elements,
		size_t offset, size_t stride, esBool normalized);
void esGeo_renderArray(const esGeo *geo, int vertices);
void esGeo_renderElements(const esGeo *geo, const esGeoBuf *indices,
		esGeoDataType dataType, int vertexCount);

// }}}
// Projection {{{

void esProj_ortho(esMat4f *mat, float x0, float y0, float x1, float y1);
void esProj_perspective(
		esMat4f *mat, float fov, float screenratio, float near, float far,
		esVec3f eye, esVec3f at, esVec3f up);

// }}}
// Texture {{{

typedef enum {
	TEX_NONE,
	TEX_LINEAR,
} esTextureMipmap;

typedef struct {
	int w, h;
	int gltexture;
} esTexture;

esErr esTexture_load(esTexture *tex, const char *file_name,
		esTextureMipmap min, esTextureMipmap mag);
void esTexture_use(esTexture *tex);
void esTexture_free(esTexture *tex);

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

esErr esFont_create(esFont *ft, esTexture *tex, esShader *shad,
		int attrib_loc, int attrib_uv, int addition_attribs);
void esFont_delete(esFont *ft);
void esFont_addText(esFont *ft, float offset_x, float offset_y,
		const char *fmt, ...);
void esFont_render(esFont *ft);
void esFont_clearBuf(esFont *ft);

// }}}
// Framebuffer {{{

typedef struct {
	int dimension;
	int gl_fb, gl_tex, gl_depth;
} esFb;

esErr esFb_create(esFb *fb, int dimension,
		esTextureMipmap min, esTextureMipmap mag);
void esFb_free(esFb *fb);
void esFb_set(esFb *fb);
void esFb_unSet(void);
void esFb_bind(esFb *fb);

// }}}
// Audio {{{

typedef struct {
	Mix_Chunk *chunk;
} esSound;

esErr esSound_create(esSound *sn, const char *file_name);
void esSound_free(esSound *sn);
void esSound_play(esSound *sn);

typedef struct {
	Mix_Music *music;
} esMusic;

esErr esMusic_create(esMusic *mu, const char *file_name);
void esMusic_free(esMusic *mu);
void esMusic_play(esMusic *mu);
void esMusic_halt(void);

// }}}
// Sprites {{{

#define ES_SPRITE_FLIPX 1
#define ES_SPRITE_FLIPY 2
#define ES_SPRITE_FLIPXY (ES_SPRITE_FLIPX | ES_SPRITE_FLIPY)

void esSprites2d_init(int palettDim, int maxSprites);
void esSprites2d_clear(void);
void esSprites2d_put(float x, float y, float radius,
		float rotate, int spriteX, int spriteY, unsigned flags);
void esSprites2d_prepear(void);
void esSprites2d_render(void);

// }}}

#endif

