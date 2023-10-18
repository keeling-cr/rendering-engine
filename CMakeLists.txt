cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

project(nica)

set(CMAKE_CXX_STANDARD 17)
set(nica ${CMAKE_CURRENT_SOURCE_DIR})

#TODO(liqining): Remove this option to third_party
add_definitions(-DV8_COMPRESS_POINTERS)
add_definitions(-DV8_ENABLE_SANDBOX)

option(ARCH "Target architecture" "")
if(CMAKE_BUILD_TYPE STREQUAL "debug")
    set(BUILD_TYPE debug)
else()
    set(BUILD_TYPE release)
endif()
ExternalProject_Add(third_party
    PREFIX ${CMAKE_BINARY_DIR}/third_party
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party

    CMAKE_ARGS
        -DBUILD_TYPE=${BUILD_TYPE}
        -DARCH=${ARCH}
    
    INSTALL_COMMAND ""
)

find_library(OPENGL_LIBRARY OpenGL)

include_directories(
    ${nica}
    ${nica}/base
    ${nica}/third_party/gen/v8/include
    ${nica}/third_party/gen/angle/include
)

add_subdirectory(angle)
add_subdirectory(base)
add_subdirectory(core)
add_subdirectory(binding)
add_subdirectory(platform)

link_directories(
    ${nica}/third_party/gen/v8/libs
)

set(V8_LIBS
    v8_libbase.a
    v8_libplatform.a
    v8_monolith.a)

add_executable(${PROJECT_NAME} ${nica}/main.cc)

target_link_libraries(${PROJECT_NAME} 
    ${V8_LIBS}
    ${OPENGL_LIBRARY}
    libnicaplatform
    libbase
    libcore
    libv8bind
    libangleutil)

