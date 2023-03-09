set(
    CLIENT_DIRS
    include/client_ui.h
    include/sys_tick.h
    include/light_spin_slider.h
    include/delay_spin_slider.h
    include/gain_spin_slider.h
    include/exposure_spin_slider.h
    src/client_ui.cpp
    src/light_spin_slider.cpp
    src/delay_spin_slider.cpp
    src/gain_spin_slider.cpp
    src/exposure_spin_slider.cpp
    src/client_ui.ui
    src/light_spin_slider.ui
    src/delay_spin_slider.ui
    src/gain_spin_slider.ui
    src/exposure_spin_slider.ui
)

add_library(
    client_ui
    STATIC ${CLIENT_DIRS}
)

target_include_directories(
    client_ui
    PUBLIC include
)

target_link_libraries(
    client_ui
    PUBLIC ua_client font_icon
)
