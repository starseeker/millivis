set(LITE_SOURCES
    Lite.cpp
    LiteBounded.cpp
    LiteShape.cpp
    LiteRect.cpp
    LiteGroup.cpp
    LiteLabel.cpp
    LiteTransform.cpp
    LiteWindow.cpp
    LiteProxy.cpp
    LiteSaveUnder.cpp
    LiteFrame.cpp
    LiteInter.cpp
    LiteEnterLeave.cpp
    LiteEnterLeaveShape.cpp
    LiteMover.cpp
    LiteMenu.cpp
    LiteMenuSimple.cpp
    LiteBox.cpp
    LiteSlider.cpp
    LiteThumb.cpp
    LiteSliderExt.cpp
    LiteRangeSlider.cpp
    LiteButton.cpp
    LiteComboBox.cpp
    LiteSwitch.cpp
    LiteTabbed.cpp
    LiteToggle.cpp
    LiteToggleButton.cpp
    LiteBackground.cpp
    LiteDisplacedLabel.cpp
    LiteTextField.cpp
    # LiteColorChooser.cpp
    Layout.cpp
    LayoutBox.cpp
    LayoutTable.cpp
    LayoutStack.cpp
    Border.cpp
    LabelLayout.cpp
    LabelLayoutRect.cpp
    LabelLayoutSimple.cpp
)

add_library(liblite_lite STATIC ${LITE_SOURCES})
target_link_libraries(liblite_lite PRIVATE liblite liblite_notifiers png z freetype expat GL GLU glfw)

# TODO: Skip test targets for now - requires Boost tree traits that need modernization
#add_executable(test_lite_as_tree test_lite_as_tree.cpp)
#add_executable(test_lite_box test_lite_box.cpp)
#target_link_libraries(test_lite_box PRIVATE liblite libtree ${MILLIONVIS_LIBS})

#add_executable(test_lite_slider test_lite_slider.cpp)
#target_link_libraries(test_lite_slider PRIVATE liblite libtree ${MILLIONVIS_LIBS})
# add_executable(test_lite_color test_lite_color.cpp)
# target_link_libraries(test_lite_color PRIVATE liblite libtree ${MILLIONVIS_LIBS})
