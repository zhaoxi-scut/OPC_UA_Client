set(
    UA_CLIENT_DIRS
    include/ua_client.h
    src/ua_client.cpp
)

add_library(
    ua_client
    STATIC ${UA_CLIENT_DIRS}
)

target_include_directories(
    ua_client
    PUBLIC include
)

target_link_libraries(
    ua_client
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets opcua_cs
)
