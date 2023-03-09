find_package(open62541 REQUIRED)

set(
    OPCUA_CS_DIRS
    include/asmpro/opcua_cs.hpp
    include/asmpro/opcua_cs/argument.hpp
    include/asmpro/opcua_cs/client.hpp
    include/asmpro/opcua_cs/object.hpp
    include/asmpro/opcua_cs/server.hpp
    include/asmpro/opcua_cs/ua_utility.hpp
    include/asmpro/opcua_cs/variable.hpp
    src/argument.cpp
    src/client.cpp
    src/server.cpp
)

add_library(
    opcua_cs
    STATIC ${OPCUA_CS_DIRS}
)

target_include_directories(
    opcua_cs
    PUBLIC include
)

target_link_libraries(
    opcua_cs
    PUBLIC open62541::open62541
)
