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
target_link_libraries(libtree PRIVATE libtable png z freetype expat GL GLU glfw)

# TODO: Skip test executables for now - require additional Boost modernization
# add_executable(test_vector_as_tree test_vector_as_tree.cpp)
# add_executable(test_sum_weight_visitor test_sum_weight_visitor.cpp)
# add_executable(test_property_tree test_property_tree.cpp)
# add_executable(test_copy test_copy.cpp)
# add_executable(test_bfs_visitor test_bfs_visitor.cpp)
# add_executable(test_tree test_tree.cpp)
# add_executable(test_dir_tree test_dir_tree.cpp)
# target_link_libraries(test_dir_tree PRIVATE libtree)
# add_executable(test_xml_tree test_xml_tree.cpp)
# target_link_libraries(test_xml_tree PRIVATE libtree)

add_subdirectory(treemap)
# add_subdirectory(drawing) # commented in Jamfile
