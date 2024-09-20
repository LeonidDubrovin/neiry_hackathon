cmake_minimum_required(VERSION 3.13)

project(CapsuleClientExample)

find_library(CAPSULE_CLIENT_LIB CapsuleClient PATHS "${CMAKE_CURRENT_SOURCE_DIR}/Lib")

set(CCESources
    Source/main.cpp
)

set(RawSignalExampleSources
    Source/RawSignalExample.cpp
)

set(FilteredSignalExampleSources
    Source/FilteredSignalExample.cpp
)

set(CCEHeaders
    Include/Platforms.hpp
    Include/CClientAPI.h
)

set(ClientSources
    Source/client.cpp
)

set(ClientHeaders
    Include/client.hpp
)

add_executable(CapsuleClientExample ${CCESources} ${CCEHeaders} ${ClientSources} ${ClientHeaders})
target_include_directories(CapsuleClientExample
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/Core)


set_target_properties(CapsuleClientExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)


target_link_libraries(CapsuleClientExample ${CAPSULE_CLIENT_LIB})


if(WIN32)
    target_link_libraries(CapsuleClientExample wsock32 ws2_32)
endif()
