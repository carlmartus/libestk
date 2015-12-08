# libESTK v4.0.0 documentation

 - [Misc](#misc)
	- type [esErr](#eserr)
	- type [esBool](#esbool)
	- func [esCheckGlError](#escheckglerror)
	- func [esFileAlloc](#esfilealloc)
 - [Math](#math)
	- type [esVec2f](#esvec2)
	- type [esVec3f](#esvec3)
	- type [esMat3f](#esmat3f)
	- type [esMat4f](#esmat4f)
	- func [esMat3f_MulVec3f](#esmat3f_mulvec3f)
	- type [esQuatf](#esquatf)
	- func [esQuatf_normalize](#esquatf_normalize)
	- func [esQuatf_mul](#esquatf_mul)
	- func [esQuatf_local](#esquatf_local)
	- func [esQuatf_matrix](#esquatf_matrix)
 - [Logging](#logging)
	- type [esLogClass](#esLogClass)
	- fptr [esLogMessage](#eslogmessage)
	- func [esLogCallback](#eslogcallback)
	- func [esLog](#esLog)
	- func [esLogVersion](#esLogVersion)
	- func [esLogUnimplemented](#eslogunimplemented)
 - [Game loop](#game loop)
	- func [esGame_init](#esgame_init)
	- func [esGame_glSwap](#esgame_glswap)
	- func [esGame_loop](#esgame_loop)
	- func [esGame_loopQuit](#esgame_loopquit)
	- func [esGame_registerKey](#esgame_registerkey)
	- func [esGame_registerMouse](#esgame_registermouse)
	- func [esGame_windowSize](#esgame_windowsize)
 - [Shader](#shader)
	- type [esUniform](#esuniform)
	- type [esShaderType](#esshadertype)
	- type [esShader](#esshader)
	- func [esShader_reset](#esshader_reset)
	- func [esShader_loadFile](#esShader_loadFile)
	- func [esShader_loadText](#esShader_loadText)
	- func [esShader_compile](#esshader_compile)
	- func [esShader_dualFile](#esShader_dualFile)
	- func [esShader_dualText](#esShader_dualText)
	- func [esShader_use](#esShader_use)
	- func [esShader_free](#esshader_free)
	- func [esShader_uniformRegister](#esshader_uniformregister)
	- func [esShader_uniformGl](#esshader_uniformgl)
 - [Geometry](#geometry)
	- type [esGeoBuf](#esgeobuf)
	- type [esGeoDataType](#esgeodatatype)
	- type [esGeoBuf](#esgeobuf)
	- func [esGeoBuf_create](#esgeobuf_create)
	- func [esGeoBuf_array](#esgeobuf_array)
	- func [esGeoBuf_arraySub](#esgeobuf_arraysub)
	- func [esGeoBuf_element](#esgeobuf_element)
	- func [esGeoBuf_free](#esgeobuf_free)
	- type [esGeo](#esgeo)
	- func [esGeo_reset](#esgeo_reset)
	- func [esGeo_point](#esgeo_point)
	- func [esGeo_renderArray](#esgeo_renderarray)
	- func [esGeo_renderElements](#esgeo_renderelements)
 - [Projection](#projection)
	- func [esProj_identity](#esproj_identity)
	- func [esProj_ortho](#esproj_ortho)
	- func [esProj_perspective](#esproj_perspective)
 - [Texture](#texture)
	- type [esTextureMipmap](#estexturemipmap)
	- type [esTexture](#estexture)
	- type [esTexture_load](#estexture_load)
	- type [esTexture_use](#estexture_use)
	- type [esTexture_free](#estexture_free)
 - [Multi render](#multi render)
	- type [esMultiRender](#esmultirender)
	- func [esMultiRender_create](#esmultirender_create)
	- func [esMultiRender_bind](#esmultirender_bind)
	- func [esMultiRender_unBind](#esmultirender_unbind)
	- func [esMultiRender_bindTexture](#esmultirender_bindtexture)
	- func [esMultiRender_bindTextureWithId](#esmultirender_bindtexturewithid)
 - [Audio](#audio)
	- type [esSound](#essound)
	- func [esSound_create](#essound_create)
	- func [esSound_free](#essound_free)
	- func [esSound_play](#essound_play)
	- type [esMusic](#esmusic)
	- func [esMusic_create](#esmusic_create)
	- func [esMusic_free](#esmusic_free)
	- func [esMusic_play](#esmusic_play)
	- func [esMusic_halt](#esmusic_halt)



# Misc

## `esErr`
```c
typedef enum {
	ES_FAIL=0,
	ES_OK=1,
} esErr;
```

## `esBool`
```c
typedef enum {
	ES_FALSE = 0,
	ES_TRUE = 1,
} esBool;
```

## `esCheckGlError`
```c
void esCheckGlError(void);
```

## `esFileAlloc`
```c
void *esFileAlloc(const char *file_name);
```


# Math

## `esVec2f`
```c
typedef struct { float x, y; } esVec2f;
```

## `esVec3f`
```c
typedef struct { float x, y, z; } esVec3f;
```

## esMat3f
```c
typedef struct {
	float mat[3*3];
} esMat3f;
```

## esMat4f
```c
typedef struct {
	float mat[4*4];
} esMat4f;
```

## esMat3f_MulVec3f
```c
esVec3f esMat3f_MulVec3f(const esMat3f *m, esVec3f v);
```

## esQuatf
```c
typedef struct {
	float x, y, z, w;
} esQuatf;
```

## esQuatf_normalize
```c
esQuatf esQuatf_normalize(esQuatf q);
```

## esQuatf_mul
```c
esQuatf esQuatf_mul(esQuatf q0, esQuatf q1);
```

## esQuatf_local
```c
esQuatf esQuatf_local(esQuatf q, float angle);
```

## esQuatf_matrix
```c
void esQuatf_matrix(esMat3f *dst, esQuatf q);
```


# Logging

## `esLogClass`
```c
typedef enum {
	ES_INFO, // Information
	ES_WARN, // Warning
	ES_ERRO, // Error
	ES_DEBU, // Debug
} esLogClass;
```

## `esLogMessage`
```c
typedef void (*esLogMessage) (esLogClass class, const char *message);
```

## `esLogCallback`
```c
void esLogCallback(esLogMessage callback);
```

## `esLog`
```c
void esLog(int class, const char *fmt, ...);
```

## `esLogVersion`
```c
void esLogVersion(void);
```

## `esLogUnimplemented`
```c
void esLogUnimplemented();
```


# Game loop

## `esGame_init`
```c
void esGame_init(int screen_width, int screen_height);
```

## `esGame_glSwap`
```c
void esGame_glSwap(void);
```

## `esGame_loop`
```c
void esGame_loop(void (*frame)(float t), void (*exit)(), int frame_rate);
```

## `esGame_loopQuit`
```c
void esGame_loopQuit(void);
```

## `esGame_registerKey`
```c
void esGame_registerKey(int sdlkey, void (*callback)(int key, int down));
```

## `esGame_registerMouse`
```c
void esGame_registerMouse(void (*callback) (int button, int down, int x, int y));
```

## `esGame_windowSize`
```c
void esGame_windowSize(int *w, int *h);
```


# Shader

## `esUniform`
```c
typedef int esUniform;
```

## `esShaderType`
```c
typedef enum {
	ES_SHADER_VERT,
	ES_SHADER_FRAG,
} esShaderType;
```

## `esShader`
```c
typedef struct {
	int glProgram;
	esUniform uniforms[UNIFORMS_MAX];
	int glShaders[SHADERS_MAX];
	int shaderCount;
} esShader;
```

## `esShader_reset`
```c
void esShader_reset(esShader *shader);
```

## `esShader_loadFile`
```c
esErr esShader_loadFile(esShader *shader,
		esShaderType type, const char *fileName);
```

## `esShader_loadText`
```c
esErr esShader_loadText(esShader *shader,
		esShaderType type, const char *source);
```

## `esShader_compile`
```c
esErr esShader_compile(esShader *shader,
		const esShaderAttrib *attribs, int attribCount);
```

## `esShader_dualFile`
```c
esErr esShader_dualFile(esShader *shader,
		const char *vertFile, const char *fragFile,
		const esShaderAttrib *attribs, int attribCount);
```

## `esShader_dualText`
```c
esErr esShader_dualText(esShader *shader,
		const char *vertSource, const char *fragSource,
		const esShaderAttrib *attribs, int attribCount);
```

## `esShader_use`
```c
void esShader_use(const esShader *shader);
```

## `esShader_free`
```c
void esShader_free(esShader *shader);
```

## `esShader_uniformRegister`
```c
esErr esShader_uniformRegister(esShader *shader,
		esUniform reg, const char *name);
```

## `esShader_uniformGl`
```c
esErr esShader_uniformGl(esShader *shader, esUniform reg);
```

# Geometry

## `esGeoBuf`
```c
typedef enum {
	ES_GEOBUF_STATIC,
	ES_GEOBUF_DYNAMIC,
	ES_GEOBUF_STREAM,
} esGeoBufType;
```

## `esGeoDataType`
```c
typedef enum {
	ES_GEODATA_FLOAT,
	ES_GEODATA_INT,
	ES_GEODATA_UINT,
	ES_GEODATA_SHORT,
	ES_GEODATA_USHORT,
	ES_GEODATA_BYTE,
	ES_GEODATA_UBYTE,
} esGeoDataType;
```

## `esGeoBuf`
```c
typedef struct {
	esGeoDataType dataType;
	unsigned int glBuf;
} esGeoBuf;
```

## `esGeoBuf_create`
```c
void esGeoBuf_create(esGeoBuf *buf);
```

## `esGeoBuf_array`
```c
void esGeoBuf_array(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type);
```

## `esGeoBuf_arraySub`
```c
void esGeoBuf_arraySub(esGeoBuf *buf,
		const void *data, size_t size, int bufOffset);
```

## `esGeoBuf_element`
```c
void esGeoBuf_element(esGeoBuf *buf,
		const void *data, size_t size, esGeoBufType type);
```

## `esGeoBuf_free`
```c
void esGeoBuf_free(esGeoBuf *buf);
```

## `esGeo`
```c
typedef struct {
	int bufCount;

	struct {
		esGeoBuf *geobuf;
		esGeoDataType datatype;
		int elements;
		size_t offset, stride;
		esBool normalized;
	} buf[ES_GEOBUFS_MAX];
} esGeo;
```

## `esGeo_reset`
```c
void esGeo_reset(esGeo *geo, int bufCount);
```

## `esGeo_point`
```c
void esGeo_point(esGeo *geo, int id, esGeoBuf *geobuf,
		esGeoDataType datatype, int elements,
		size_t offset, size_t stride, esBool normalized);
```

## `esGeo_renderArray`
```c
void esGeo_renderArray(const esGeo *geo, int vertices);
```

## `esGeo_renderElements`
```c
void esGeo_renderElements(const esGeo *geo, const esGeoBuf *indices,
		esGeoDataType dataType, int vertexCount);
```


# Projection

## `esProj_identity`
```c
void esProj_identity(esMat4f *mat);
```

## `esProj_ortho`
```c
void esProj_ortho(esMat4f *mat, float x0, float y0, float x1, float y1);
```

## `esProj_perspective`
```c
void esProj_perspective(
		esMat4f *mat, float fov, float screenratio, float near, float far,
		esVec3f eye, esVec3f at, esVec3f up);
```


# Texture

## `esTextureMipmap`
```c
typedef enum {
	TEX_NONE,
	TEX_LINEAR,
} esTextureMipmap;
```

## `esTexture`
```c
typedef struct {
	int w, h;
	int gltexture;
} esTexture;
```

## `esTexture_load`
```c
esErr esTexture_load(esTexture *tex, const char *file_name,
		enum esTextureMipmap min, enum esTextureMipmap mag);
```

## `esTexture_use`
```c
void esTexture_use(esTexture *tex);
```

## `esTexture_free`
```c
void esTexture_free(esTexture *tex);
```


# Multi render

## `esMultiRender`
```c
typedef struct {
	unsigned textureCount, width, height;
	esGLuint frameBuffer, depthBuffer;
	esGLuint renderTextures[ES_FRAMEBUFFER_MAX_TEXTURES];
} esMultiRender;
```

## `esMultiRender_create`
```c
esErr esMultiRender_create(esMultiRender *mr,
		unsigned width, unsigned height,
		esTextureMipmap mipMapMin, esTextureMipmap mipMapMag,
		unsigned textureCount, ...);
```

## `esMultiRender_destroy`
```c
void esMultiRender_destroy(esMultiRender *mr);
```

## `esMultiRender_bind`
```c
void esMultiRender_bind(esMultiRender *mr);
```

## `esMultiRender_unBind`
```c
void esMultiRender_unBind(void);
```

## `esMultiRender_bindTexture`
```c
void esMultiRender_bindTexture(esMultiRender *mr, unsigned textureId);
```

## `esMultiRender_bindTextureWithId`
```c
void esMultiRender_bindTextureWithId(esMultiRender *mr, unsigned textureId,
		unsigned openGlTextureId);
```

# Audio

## `esSound`
```c
typedef struct {
	Mix_Chunk *chunk;
} esSound;
```

## `esSound_create`
```c
esErr esSound_create(esSound *sn, const char *file_name);
```

## `esSound_free`
```c
void esSound_free(esSound *sn);
```

## `esSound_play`
```c
void esSound_play(esSound *sn);
```

## `esMusic`
```c
typedef struct {
	Mix_Music *music;
} esMusic;
```

## `esMusic_create`
```c
esErr esMusic_create(esMusic *mu, const char *file_name);
```

## `esMusic_free`
```c
void esMusic_free(esMusic *mu);
```

## `esMusic_play`
```c
void esMusic_play(esMusic *mu);
```

## `esMusic_halt`
```c
void esMusic_halt(void);
```

