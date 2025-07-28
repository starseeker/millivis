set(COLORS_SOURCES
    grey.cpp
    hsb.cpp
    xyz.cpp
    uvY.cpp
    hvc.cpp
)

add_library(liblite_colors STATIC ${COLORS_SOURCES})
