cmake_minimum_required(VERSION 2.6)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_definitions("-Wall -O2 -g -std=gnu11")

# Version/Naming
# ==============

set(MAJOR 2)
set(MINOR 0)
set(PATCH 0)
set(VERSION ${MAJOR}.${MINOR}.${PATCH})
set(ESTK estk${MAJOR})

# Library
# =======

file(GLOB LIB_SOURCE src/*.c)
add_library(${ESTK} SHARED ${LIB_SOURCE})

set_target_properties(${ESTK}
	PROPERTIES VERSION ${VERSION}
	SOVERSION ${MAJOR})

target_link_libraries(${ESTK} m GLEW SDL SDL_image SDL_mixer)

