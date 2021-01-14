cmake_minimum_required(VERSION 3.6)

# Name the project
project(threadpool)

# Include MeshLib core and the 'include' of the project itself
include_directories("include")

# Also, the file(GLOB...) allows for wildcard additions
file(GLOB SRCS
    "*.h"
    "*.cpp")

# Add an executable target called MyDemo to be build from 
# the source files.
add_executable(threadpool ${SRCS})

# Install the executeable program in the bin folder of the
# current project directory.
install(TARGETS threadpool DESTINATION ${CMAKE_SOURCE_DIR}/bin)
