cmake_minimum_required(VERSION 3.12)
project(millionvis VERSION 0.2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Optionally: Define sources in subdirectories
add_subdirectory(infovis)
add_subdirectory(treemap2)
# add_subdirectory(geometric) # commented in original Jamfile
#add_subdirectory(bench)
#add_subdirectory(focusimage)

# Find system libraries (rough translation of Jamrules logic)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Freetype REQUIRED)

# Assuming OpenGL and GLUT are present for now
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${PNG_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    # Add others as needed
)

# Link libraries globally if needed (can also be local to targets)
set(MILLIONVIS_LIBS
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    # Add others as needed
)
