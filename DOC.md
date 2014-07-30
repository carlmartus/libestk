# libESTK v3.0.0 documentation

 - [Misc](#misc)
	- type [esVec2](#esvec2)
	- type [esVec3](#esvec3)
	- type [esBool](#esbool)
	- func [esCheckGlError](#escheckglerror)
	- func [esFileAlloc](#esfilealloc)
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
	- func [esGame_windowSize](#esgame_windowsize)



# Misc

## `esVec2`
```c
typedef struct { float x, y; } esVec2;
```

## `esVec3`
```c
typedef struct { float x, y, z; } esVec3;
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

## `esGame_windowSize`
```c
void esGame_windowSize(int *w, int *h);
```

