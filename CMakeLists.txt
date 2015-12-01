cmake_minimum_required(VERSION 2.6)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/demos)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_definitions("-Wall -O2 -std=gnu11")

# Version/Naming
# ==============

set(MAJOR 5)
set(MINOR 0)
set(PATCH 0)
set(NOTE "-dev")
set(VERSION ${MAJOR}.${MINOR}.${PATCH}${NOTE})
set(ESTK estk${MAJOR})


# Configuration
# =============

configure_file (
	"${PROJECT_SOURCE_DIR}/estk.h.in"
	"${PROJECT_BINARY_DIR}/estk.h"
	)

include_directories ("${PROJECT_BINARY_DIR}")


# Library
# =======

#file(GLOB LIB_SOURCE src/*.c)
set(LIB_SOURCE
	src/fb.c
	src/font.c
	src/gameloop.c
	src/geometry.c
	src/log.c
	src/math.c
	src/misc.c
	src/projection.c
	src/shader.c
	src/sound.c
	src/sprites.c
	src/texture.c
	)

add_library(${ESTK} STATIC ${LIB_SOURCE})

set_target_properties(${ESTK}
	PROPERTIES VERSION ${VERSION}
	SOVERSION ${MAJOR})

target_link_libraries(${ESTK} m GLEW SDL SDL_image SDL_mixer)

# Demos
# =====

set(DEMOS_SOURCE
	attr
	cam
	clear
	events
	fb
	font
	image
	mainloop
	quaternion
	red
	shadercomp
	sound
	sprites
	stride
	uniform
	)

foreach(FILE ${DEMOS_SOURCE})
	add_executable(${FILE} "demos/${FILE}.c")
	target_link_libraries(${FILE} ${ESTK} GL)
	include_directories(${FILE} src)
endforeach(FILE)

# Install
# =======

install(TARGETS ${ESTK}
	LIBRARY DESTINATION "lib"
	ARCHIVE DESTINATION "lib"
	#RUNTIME DESTINATION "bin" # Don't install demo programs
	COMPONENT library)
install(FILES
	"${PROJECT_BINARY_DIR}/estk.h"
	DESTINATION include)

