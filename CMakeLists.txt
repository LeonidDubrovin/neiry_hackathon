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

add_executable(CapsuleClientExample ${CCESources} ${CCEHeaders})
target_include_directories(CapsuleClientExample
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/Core)

add_executable(CapsuleRawSignalExample ${RawSignalExampleSources} ${CCEHeaders})
target_include_directories(CapsuleRawSignalExample
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/Core)

add_executable(CapsuleFilteredSignalExample ${FilteredSignalExampleSources} ${CCEHeaders})
target_include_directories(CapsuleFilteredSignalExample
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include/Core)


set_target_properties(CapsuleClientExample CapsuleRawSignalExample CapsuleFilteredSignalExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON)

if (APPLE)
    set_target_properties(CapsuleClientExample CapsuleRawSignalExample CapsuleFilteredSignalExample PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@executable_path/")
endif()

target_link_libraries(CapsuleClientExample ${CAPSULE_CLIENT_LIB})
target_link_libraries(CapsuleRawSignalExample ${CAPSULE_CLIENT_LIB})
target_link_libraries(CapsuleFilteredSignalExample ${CAPSULE_CLIENT_LIB})
