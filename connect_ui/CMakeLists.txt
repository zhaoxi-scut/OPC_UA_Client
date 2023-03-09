set(
    CONNECT_DIRS
    include/connect_ui.h
    src/connect_ui.cpp
    src/connect_ui.ui
)

add_library(
    connect_ui
    STATIC ${CONNECT_DIRS}
)

target_include_directories(
    connect_ui
    PUBLIC include
)

target_link_libraries(
    connect_ui
    PUBLIC ua_client font_icon
)
