set(TREEMAP2_SOURCES
    FileType.cpp
    Properties.cpp
    ControlsTab.cpp
    LitePath.cpp
    LiteTreemap.cpp
    # LiteScatterPlot.cpp
    LayoutVisu.cpp
    LayoutVisuSQ.cpp
    LayoutVisuSD.cpp
    LayoutVisuSP.cpp
    LiteSpeed.cpp
    treemap2.cpp
    # nv_3d_drawer.cpp
    FastDrawer.cpp
    FastPicker.cpp
    ColorRamp.cpp
    Recorder.cpp
    BarGraph.cpp
    LiteRangeSliderGraph.cpp
    AnimateTree.cpp
    # DynaQueries.cpp
    LabelTreemap.cpp
    # New font backend
    StrueTypeFont.cpp
    installStrueTypeFont.cpp
    font_backend/struetype.c
    font_backend/fontstash.c
)

add_executable(treemap2 ${TREEMAP2_SOURCES})

# Assuming liblite and libtree are built elsewhere in the project
target_link_libraries(treemap2 PRIVATE 
    liblite liblite_lite liblite_inter liblite_notifiers liblite_colors 
    libtree libtable 
    png z expat GL GLU glut)
target_include_directories(treemap2 PRIVATE ${CMAKE_SOURCE_DIR})
# TODO: Add current directory to include path for local headers - C++17 modernization
target_include_directories(treemap2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
