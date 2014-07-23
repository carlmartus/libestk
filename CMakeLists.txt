cmake_minimum_required(VERSION 2.6)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_definitions("-Wall -O2 -g -pedantic -std=gnu11")

# Version/Naming
# ==============

set(MAJOR 2)
set(MINOR 0)
set(PATCH 0)
set(VERSION ${MAJOR}.${MINOR}.${PATCH})
set(ESTK organic${MAJOR})

# Library
# =======

file(GLOB LIB_SOURCE src/*.c)
add_library(${ORGANIC} SHARED ${LIB_SOURCE})

set_target_properties(${ORGANIC}
	PROPERTIES VERSION ${VERSION}
	SOVERSION ${MAJOR})

