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

# Link liblite with the required libraries and include directories
target_link_libraries(liblite PRIVATE png z freetype expat GL GLU glut)
target_include_directories(liblite PRIVATE ${CMAKE_SOURCE_DIR})
# TODO: Add explicit freetype2 include for C++17 modernization
target_include_directories(liblite PRIVATE /usr/include/freetype2)

# TODO: Skip test executables for now to focus on main targets - will address concept checking later
# add_executable(test_box test_box.cpp)
# target_link_libraries(test_box PRIVATE liblite ${MILLIONVIS_LIBS})

# add_executable(test_point_vec test_point_vec.cpp)
# target_link_libraries(test_point_vec PRIVATE liblite ${MILLIONVIS_LIBS})

# add_executable(test_color test_color.cpp)
# target_link_libraries(test_color PRIVATE liblite ${MILLIONVIS_LIBS})

# add_executable(test_lite test_lite.cpp)
# target_link_libraries(test_lite PRIVATE liblite ${MILLIONVIS_LIBS})

# add_executable(test_lite_pick test_lite_pick.cpp)
# target_link_libraries(test_lite_pick PRIVATE liblite ${MILLIONVIS_LIBS})

# add_executable(test_font test_font.cpp)
# target_link_libraries(test_font PRIVATE liblite ${MILLIONVIS_LIBS})

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

