add_subdirectory (Messages)
add_library(OpenFlow OpenFlowTable.cpp)
target_link_libraries(OpenFlow Messages)
