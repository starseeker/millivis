set(DRAWING_SOURCES
    drawing.cpp
    Font.cpp
    FontGlut.cpp
    FontWin32.cpp
    FontFT.cpp
    Image.cpp
    ImagePNG.cpp
    SaveUnder.cpp
    Texture.cpp
    Transform.cpp
    Animate.cpp
    AnimateBoxList.cpp
    AnimateCompose.cpp
    AnimateInverse.cpp
)

add_library(liblite STATIC ${DRAWING_SOURCES})

add_executable(test_box test_box.cpp)
add_executable(test_point_vec test_point_vec.cpp)
# add_executable(test_color test_color.cpp)

add_executable(test_lite test_lite.cpp)
add_executable(test_lite_pick test_lite_pick.cpp)
add_executable(test_font test_font.cpp)

# Add dependencies for test_lite_box and test_lite_slider
#add_executable(test_lite_box test_lite_box.cpp)
#target_link_libraries(test_lite_box PRIVATE liblite libtree ${MILLIONVIS_LIBS})

#add_executable(test_lite_slider test_lite_slider.cpp)
#target_link_libraries(test_lite_slider PRIVATE liblite libtree ${MILLIONVIS_LIBS})

# add_executable(test_lite_color test_lite_color.cpp)
# target_link_libraries(test_lite_color PRIVATE liblite libtree ${MILLIONVIS_LIBS})

add_subdirectory(colors)
add_subdirectory(lite)
add_subdirectory(inter)
add_subdirectory(notifiers)

