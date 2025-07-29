# Geometric test executable

add_executable(test_geometric test_geometric.cpp)
target_include_directories(test_geometric PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_geometric PRIVATE liblite libtree png z freetype expat GL GLU glut)