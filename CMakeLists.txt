project(nica)

set(CMAKE_CXX_STANDARD 17)
set(nica ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DV8_COMPRESS_POINTERS)
add_definitions(-DV8_ENABLE_SANDBOX)

include_directories(
    ${nica}
    ${nica}/base
    ${nica}/third_party/v8/include
    ${nica}/third_party/test
)

add_subdirectory(base)
add_subdirectory(core)

link_directories(
    ${nica}/third_party/v8/libs
)

set(V8_LIBS
    v8_libbase.a
    v8_libplatform.a
    v8_monolith.a)

add_executable(${PROJECT_NAME} ${nica}/test.cc)

target_link_libraries(${PROJECT_NAME} 
    ${V8_LIBS}
    libbase
    libcore)

