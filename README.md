libestk
=======

OpenGL ES Toolkit v4.0.0.

This project is a continuation of [ESTK](https://github.com/carlmartus/estk) which can be considered version 1.0 of libestk.

Documentation can be found [here](DOC.md).

# Features
 * FPS bound game loop
 * OpenGL ES Compatibility
 * WebGL compatibility
 * Emscripten compatibility
 * GLSL Shaders wrapper
 * OpenGL vertex buffer wrapper
 * Projection matrix generators
 * Texture file loading
 * Bitmap font rendering
 * 2D sprite rendering
 * Quaternion rotations
 * Keyboard/Mouse event wrapper

# Usage

## Dependencies
 * SDL
 * cmake
 * OpenGL


## Compilation
  First generate the build environment with cmake:
```
cmake .
```


  Now build the projet with:
```
make
```

The library and all the demos can now be found in the ```bin``` directory.


If you wish to add the library to your system run this:
```
make install
```


Todo
====

