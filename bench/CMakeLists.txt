# Bench test executables

add_executable(raw_filter raw_filter.cpp)
target_link_libraries(raw_filter PRIVATE ${MILLIONVIS_LIBS})

add_executable(raw_disp_rect raw_disp_rect.cpp)
target_link_libraries(raw_disp_rect PRIVATE liblite liblite_colors libtree libtable ${MILLIONVIS_LIBS})

add_executable(disp_lines disp_lines.cpp)
target_link_libraries(disp_lines PRIVATE libtree liblite ${MILLIONVIS_LIBS})