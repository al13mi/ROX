add_library(System LookupTree.cpp)
add_library(MacLayer MacLayer.cpp)
target_link_libraries(System MacLayer)

