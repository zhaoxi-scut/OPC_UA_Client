add_library(
    font_icon INTERFACE
)

target_include_directories(
    font_icon
    INTERFACE include
)

target_link_libraries(
    font_icon
    INTERFACE Qt${QT_MAJOR_VERSION}::Widgets
)
