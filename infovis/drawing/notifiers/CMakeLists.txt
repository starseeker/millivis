set(NOTIFIERS_SOURCES
    BeginEnd.cpp
    BoundedRange.cpp
    Change.cpp
)

add_library(liblite_notifiers STATIC ${NOTIFIERS_SOURCES})
