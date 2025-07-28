add_executable(test_gl_drawer test_gl_drawer.cpp)
target_link_libraries(test_gl_drawer PRIVATE liblite libtree ${MILLIONVIS_LIBS})

# add_executable(test_gl_cs_drawer test_gl_cs_drawer.cpp)
add_executable(test_gl_pick test_gl_pick.cpp)
target_link_libraries(test_gl_pick PRIVATE liblite libtree ${MILLIONVIS_LIBS})

add_executable(test_lite_drawer test_lite_drawer.cpp)
target_link_libraries(test_lite_drawer PRIVATE liblite libtree ${MILLIONVIS_LIBS})

add_executable(test_lite_drawer2 test_lite_drawer2.cpp)  
target_link_libraries(test_lite_drawer2 PRIVATE liblite libtree ${MILLIONVIS_LIBS})

add_executable(test_lite_drawer3 test_lite_drawer3.cpp)
target_link_libraries(test_lite_drawer3 PRIVATE liblite libtree ${MILLIONVIS_LIBS})

# add_executable(test_3d_drawer test_3d_drawer.cpp)
