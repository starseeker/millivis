set(DRAWING_SOURCES
    drawing.cpp
    Font.cpp
    installStrueTypeFont.cpp
    StrueTypeFont.cpp
    font_backend/struetype.c
    font_backend/fontstash.c
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
target_link_libraries(liblite PRIVATE png z expat GL GLU glut)
target_include_directories(liblite PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(test_box test_box.cpp)
target_link_libraries(test_box PRIVATE liblite ${MILLIONVIS_LIBS})

add_executable(test_point_vec test_point_vec.cpp)
target_link_libraries(test_point_vec PRIVATE liblite ${MILLIONVIS_LIBS})

add_executable(test_color test_color.cpp)
target_link_libraries(test_color PRIVATE liblite ${MILLIONVIS_LIBS})

add_executable(test_lite test_lite.cpp)
target_link_libraries(test_lite PRIVATE liblite liblite_lite liblite_inter ${MILLIONVIS_LIBS})

add_executable(test_lite_pick test_lite_pick.cpp)
target_link_libraries(test_lite_pick PRIVATE liblite liblite_lite liblite_inter ${MILLIONVIS_LIBS})

add_executable(test_font test_font.cpp)
target_link_libraries(test_font PRIVATE liblite liblite_lite liblite_inter ${MILLIONVIS_LIBS})

# Note: test_lite_* executables are defined in the lite subdirectory

add_subdirectory(colors)
add_subdirectory(lite)
add_subdirectory(inter)
add_subdirectory(notifiers)

