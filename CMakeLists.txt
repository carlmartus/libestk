cmake_minimum_required(VERSION 3.0)
project(estk C)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Configuration
# =============

option(ES_OPT_AUDIO "Audio with SDL_mixer" YES)
option(ES_OPT_IMAGE "Image loading with SDL_image" YES)
option(ES_OPT_EMSCRIPTEN "Emscripten mode" NO)

# Version/Naming
# ==============

set(MAJOR 6)
set(MINOR 1)
set(PATCH 0)
set(NOTE "-dev")
set(VERSION ${MAJOR}.${MINOR}.${PATCH}${NOTE})
set(ESTK estk${MAJOR})

# Configuration
# =============

if(NOT ES_OPT_EMSCRIPTEN)
	find_package(GLEW REQUIRED)
endif(NOT ES_OPT_EMSCRIPTEN)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

configure_file (
	"src/estk.h.in"
	"${PROJECT_BINARY_DIR}/estk.h"
	)

include_directories ("${PROJECT_BINARY_DIR}")

# Library
# =======

#file(GLOB LIB_SOURCE src/*.c)
set(LIB_SOURCE
	src/gameloop.c
	src/geometry.c
	src/log.c
	src/math.c
	src/misc.c
	src/multirender.c
	src/projection.c
	src/shader.c
	src/texture.c
	)

if(ES_OPT_AUDIO)
	set(LIB_SOURCE ${LIB_SOURCE} src/sound.c)
endif(ES_OPT_AUDIO)

add_library(${ESTK} STATIC ${LIB_SOURCE})
set_property(TARGET ${ESTK} PROPERTY C_STANDARD 11) # Use GNU C11

set_target_properties(${ESTK}
	PROPERTIES VERSION ${VERSION}
	SOVERSION ${MAJOR})

if(NOT ES_OPT_EMSCRIPTEN)
	target_link_libraries(${ESTK} m GLEW SDL2)
endif(NOT ES_OPT_EMSCRIPTEN)

if(ES_OPT_AUDIO AND NOT ES_OPT_EMSCRIPTEN)
	target_link_libraries(${ESTK} SDL2_mixer)
endif(ES_OPT_AUDIO AND NOT ES_OPT_EMSCRIPTEN)

if(ES_OPT_IMAGE AND NOT ES_OPT_EMSCRIPTEN)
	target_link_libraries(${ESTK} SDL2_image)
endif(ES_OPT_IMAGE AND NOT ES_OPT_EMSCRIPTEN)

# Samples
# =======

set(SAMPLES_SOURCE
	attr
	cam
	events
	mainloop
	multirender
	quaternion
	red
	stride
	uniform
	)

if(ES_OPT_AUDIO)
	set(SAMPLES_SOURCE ${SAMPLES_SOURCE} sound)
endif(ES_OPT_AUDIO)

if(ES_OPT_IMAGE)
	set(SAMPLES_SOURCE ${SAMPLES_SOURCE} image)
endif(ES_OPT_IMAGE)

# No samples for emscripen since it's just .js files
if(NOT ES_OPT_EMSCRIPTEN)
	foreach(FILE ${SAMPLES_SOURCE})
		add_executable(${FILE} "samples/${FILE}.c")
		target_link_libraries(${FILE} ${ESTK} GL)
		include_directories(${FILE} src)
	endforeach(FILE)
endif(NOT ES_OPT_EMSCRIPTEN)

# Install
# =======

install(TARGETS ${ESTK}
	LIBRARY DESTINATION "lib"
	ARCHIVE DESTINATION "lib"
	#RUNTIME DESTINATION "bin" # Don't install sample programs
	COMPONENT library)
install(FILES
	"${PROJECT_BINARY_DIR}/estk.h"
	DESTINATION include)

