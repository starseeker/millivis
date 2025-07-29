# Focus image test executable

add_executable(focusimage focusimage.cpp)
target_link_libraries(focusimage PRIVATE liblite liblite_lite liblite_inter libtree png z freetype expat GL GLU glut)