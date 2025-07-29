set(TREE_SOURCES
    tree.cpp
    dir_tree.cpp
    xml_tree.cpp
    xmltree_tree.cpp
    export_tree_xml.cpp
    dir_property_tree.cpp
    xml_property_tree.cpp
    ObservableTree.cpp
)

add_library(libtree STATIC ${TREE_SOURCES})
target_link_libraries(libtree PRIVATE libtable png z freetype expat GL GLU glut)

# Basic tree tests - now enabled for C++17
add_executable(test_tree test_tree.cpp)
target_link_libraries(test_tree PRIVATE libtree ${MILLIONVIS_LIBS})

add_executable(test_dir_tree test_dir_tree.cpp)
target_link_libraries(test_dir_tree PRIVATE libtree ${MILLIONVIS_LIBS})

add_executable(test_xml_tree test_xml_tree.cpp)
target_link_libraries(test_xml_tree PRIVATE libtree ${MILLIONVIS_LIBS})

# Additional tree tests - enable after basic tests work
add_executable(test_copy test_copy.cpp)
target_link_libraries(test_copy PRIVATE libtree ${MILLIONVIS_LIBS})

add_executable(test_bfs_visitor test_bfs_visitor.cpp)
target_link_libraries(test_bfs_visitor PRIVATE libtree ${MILLIONVIS_LIBS})

add_executable(test_sum_weight_visitor test_sum_weight_visitor.cpp)
target_link_libraries(test_sum_weight_visitor PRIVATE libtree ${MILLIONVIS_LIBS})

add_subdirectory(treemap)
# add_subdirectory(drawing) # commented in Jamfile
