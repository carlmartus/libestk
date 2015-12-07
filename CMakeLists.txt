cmake_minimum_required(VERSION 2.6)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/samples)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

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

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(GLEW REQUIRED)

configure_file (
	"${PROJECT_SOURCE_DIR}/estk.h.in"
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
	src/sound.c
	src/texture.c
	)

add_library(${ESTK} STATIC ${LIB_SOURCE})
set_property(TARGET ${ESTK} PROPERTY C_STANDARD 11) # Use GNU C11

set_target_properties(${ESTK}
	PROPERTIES VERSION ${VERSION}
	SOVERSION ${MAJOR})

target_link_libraries(${ESTK} m GLEW SDL SDL_image SDL_mixer)

# Samples
# =======

set(SAMPLES_SOURCE
	attr
	cam
	clear
	events
	image
	mainloop
	quaternion
	red
	shadercomp
	sound
	stride
	uniform
	)

foreach(FILE ${SAMPLES_SOURCE})
	add_executable(${FILE} "samples/${FILE}.c")
	target_link_libraries(${FILE} ${ESTK} GL)
	include_directories(${FILE} src)
endforeach(FILE)

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

