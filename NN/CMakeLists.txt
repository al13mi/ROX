add_library(NeuralNetwork NeuralNetwork.cpp)

if(NOT TYPE MATCHES COV)
add_library(Brain brain.capnp.cpp)
target_link_libraries(NeuralNetwork Brain)
endif()
