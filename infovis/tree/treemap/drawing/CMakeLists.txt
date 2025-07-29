# Treemap test executables - C++17 modernized

add_executable(test_gl_drawer test_gl_drawer.cpp)
target_link_libraries(test_gl_drawer PRIVATE liblite libtree libtable
    png z freetype expat GL GLU glfw)

# TODO: test_gl_cs_drawer requires gl_cs_drawer.hpp C++17 modernization - skip for now
# add_executable(test_gl_cs_drawer test_gl_cs_drawer.cpp)
# target_link_libraries(test_gl_cs_drawer PRIVATE liblite libtree libtable
#     png z freetype expat GL GLU glfw)

add_executable(test_gl_pick test_gl_pick.cpp)
target_link_libraries(test_gl_pick PRIVATE liblite libtree libtable
    png z freetype expat GL GLU glfw)

add_executable(test_lite_drawer test_lite_drawer.cpp)
target_link_libraries(test_lite_drawer PRIVATE 
    liblite liblite_lite liblite_inter liblite_notifiers liblite_colors 
    libtree libtable
    ${MILLIONVIS_LIBS})

add_executable(test_lite_drawer2 test_lite_drawer2.cpp)  
target_link_libraries(test_lite_drawer2 PRIVATE 
    liblite liblite_lite liblite_inter liblite_notifiers liblite_colors 
    libtree libtable
    ${MILLIONVIS_LIBS})

add_executable(test_lite_drawer3 test_lite_drawer3.cpp)
target_link_libraries(test_lite_drawer3 PRIVATE 
    liblite liblite_lite liblite_inter liblite_notifiers liblite_colors 
    libtree libtable
    ${MILLIONVIS_LIBS})

# add_executable(test_3d_drawer test_3d_drawer.cpp)
