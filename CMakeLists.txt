cmake_minimum_required(VERSION 3.6)

# Name the project
project(multithread_learning_cplusplus)

# Include MeshLib core and the 'include' of the project itself
include_directories("include")

# Also, the file(GLOB...) allows for wildcard additions
file(GLOB SRCS
    "include/*.h"
    "src/*.cpp")

# Add an executable target called MyDemo to be build from 
# the source files.
add_executable(multithread_learning_cplusplus ${SRCS})

# Install the executeable program in the bin folder of the
# current project directory.
install(TARGETS multithread_learning_cplusplus DESTINATION ${CMAKE_SOURCE_DIR}/bin)
