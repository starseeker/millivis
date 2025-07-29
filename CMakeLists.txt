cmake_minimum_required(VERSION 3.12)

# Set modern CMake policies
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(millionvis VERSION 0.2)

add_definitions(-g)

# Set C++ standard to C++17 for modernization
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure the config.hpp file
configure_file(infovis/config.hpp.in infovis/config.hpp @ONLY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Optionally: Define sources in subdirectories
add_subdirectory(infovis)
add_subdirectory(treemap2)
# add_subdirectory(geometric) # Has complex template issues requiring major refactoring
add_subdirectory(bench)
add_subdirectory(focusimage)

# Find system libraries (rough translation of Jamrules logic)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Freetype REQUIRED)
find_package(EXPAT REQUIRED)

# Assuming OpenGL and GLUT are present for now
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PNG_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${Freetype_INCLUDE_DIRS}
    ${EXPAT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    # Add others as needed
)

# Link libraries globally if needed (can also be local to targets)
set(MILLIONVIS_LIBS
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${Freetype_LIBRARIES}
    ${EXPAT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    # Add others as needed
    CACHE INTERNAL "MillionVis external libraries"
)
