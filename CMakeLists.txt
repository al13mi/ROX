cmake_minimum_required (VERSION 2.6)
project (ROX)

set (ROX_VERSION_MAJOR 1)
set (ROX_VERSION_MINOR 0)

set (CMAKE_CXX_FLAGS "-pthread -I. -g -msse4.2 -std=gnu++11 -Wall -Werror -pedantic -fsanitize=address -fsanitize=leak -fsanitize=undefined")
set (CMAKE_EXE_LINKER_FLAGS "-g -levent -std=gnu++11 -lcapnp -lcapnp-rpc -lkj -lkj-async -pedantic -fsanitize=address -fsanitize=leak -fsanitize=undefined")

include_directories("${PROJECT_BINARY_DIR}")

add_executable (ROX main.cpp)
add_subdirectory (OpenFlow)
add_subdirectory (NN)
add_subdirectory (System)
add_subdirectory (Network)
add_library(Controller Controller.cpp)
target_link_libraries (ROX System Controller OpenFlow Network NeuralNetwork)
