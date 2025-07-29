# Focus image test executable

add_executable(focusimage focusimage.cpp)
target_link_libraries(focusimage PRIVATE liblite libtree ${MILLIONVIS_LIBS})