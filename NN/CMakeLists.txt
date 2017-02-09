add_library(Brain brain.capnp.cpp)
add_library(NeuralNetwork NeuralNetwork.cpp)
target_link_libraries(NeuralNetwork Brain)
