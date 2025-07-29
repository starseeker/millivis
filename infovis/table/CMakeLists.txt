set(TABLE_SOURCES
    column.cpp
    table.cpp
    metadata.cpp
    csv_loader.cpp
)

add_library(libtable STATIC ${TABLE_SOURCES})
target_link_libraries(libtable PRIVATE ${MILLIONVIS_LIBS})

add_executable(test_column test_column.cpp)
target_link_libraries(test_column PRIVATE libtable)

add_executable(test_table test_table.cpp)
target_link_libraries(test_table PRIVATE libtable)
