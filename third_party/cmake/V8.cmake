file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/v8)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/gen/v8)

set(V8_FOLDER ${CMAKE_SOURCE_DIR}/v8)
set(V8_OUTPUT_FOLDER ${CMAKE_SOURCE_DIR}/gen/v8)
set(V8_SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/v8/v8)

set(DEPOT_TOOLS_CONFIG_COMMAND git config core.autocrlf false)
thirdparty_command(
    NAME depot-tools-config
    COMMAND ${DEPOT_TOOLS_CONFIG_COMMAND}
    WORKING_DIRECTORY ${DEPOT_TOOLS_PATH}
    DEPENDS depot-tools
)

set(V8_FETCH_COMMAND fetch --no-history v8)
thirdparty_command(
    NAME fetch-v8
    COMMAND ${V8_FETCH_COMMAND}
    WORKING_DIRECTORY ${V8_FOLDER}
)

if (UNIX AND NOT APPLE)
    set(V8_INSTALL_DEPS ./build/install-build-deps.sh)
    thirdparty_command(
        NAME install-deps
        COMMAND ${V8_INSTALL_DEPS}
        WORKING_DIRECTORY ${V8_FOLDER}
        DEPENDS fetch-v8
)
endif()

list(APPEND V8_GEN_ARGS
    is_component_build = false
    dcheck_always_on = false
    use_custom_libcxx = false
    v8_monolithic = true
    v8_use_external_startup_data = false
)

if(BUILD_TYPE STREQUAL "debug")
    list(APPEND V8_GEN_ARGS
        is_debug = true
    )
else()
    list(APPEND V8_GEN_ARGS
        is_debug = false
    )
endif()

if(ARCH STREQUAL "x86")
    list(APPEND V8_GEN_ARGS
        target_cpu = \"x86\"
    )
else()
    list(APPEND V8_GEN_ARGS
        target_cpu = \"x64\"
    )
endif()

set(V8_CONFIGURE_COMMAND gn gen out/${BUILD_TYPE} --args='${V8_GEN_ARGS}')
thirdparty_command(
    NAME gen
    COMMAND ${V8_CONFIGURE_COMMAND}
    WORKING_DIRECTORY ${V8_SOURCE_FOLDER}
    #DEPENDS fetch-v8
)


set(V8_BUILD_COMMAND 
    ninja -C out/${BUILD_TYPE} v8_monolith v8_libbase v8_libplatform)
thirdparty_command(
    NAME ninja
    COMMAND ${V8_BUILD_COMMAND}
    WORKING_DIRECTORY ${V8_SOURCE_FOLDER}
    DEPENDS gen
)

if (UNIX AND NOT APPLE)
    MESSAGE(STATUS "unix")
elseif (WIN32)
    MESSAGE(STATUS "windows")
elseif (APPLE)
    set(V8_OUTPUT_INCLUDE_COMMAND cp -r ${V8_SOURCE_FOLDER}/include ${V8_OUTPUT_FOLDER}/)
    thirdparty_command(
        NAME output_header
        COMMAND ${V8_OUTPUT_INCLUDE_COMMAND}
        WORKING_DIRECTORY ${V8_SOURCE_FOLDER}
        DEPENDS ninja
    )
    
    set(V8_STRIP_LIBS
        strip ${V8_SOURCE_FOLDER}/out/${BUILD_TYPE}/obj/libv8_libbase.a
        && strip ${V8_SOURCE_FOLDER}/out/${BUILD_TYPE}/obj/libv8_libplatform.a
        && strip ${V8_SOURCE_FOLDER}/out/${BUILD_TYPE}/obj/libv8_monolith.a   
    )
    thirdparty_command(
        NAME strip_libs
        COMMAND ${V8_STRIP_LIBS}
        WORKING_DIRECTORY ${V8_SOURCE_FOLDER}
        DEPENDS ninja
    )

    file(MAKE_DIRECTORY ${V8_OUTPUT_FOLDER}/libs)
    set(V8_OUTPUT_LIB_COMMAND
        cp ${V8_SOURCE_FOLDER}/out/${BUILD_TYPE}/obj/libv8_libbase.a ${V8_OUTPUT_FOLDER}/libs
        && cp ${V8_SOURCE_FOLDER}/out/${BUILD_TYPE}/obj/libv8_libplatform.a ${V8_OUTPUT_FOLDER}/libs
        && cp ${V8_SOURCE_FOLDER}/out/${BUILD_TYPE}/obj/libv8_monolith.a ${V8_OUTPUT_FOLDER}/libs
    )
    thirdparty_command(
        NAME output_libs
        COMMAND ${V8_OUTPUT_LIB_COMMAND}
        WORKING_DIRECTORY ${V8_SOURCE_FOLDER}
        DEPENDS strip_libs 
    )
else ()
    MESSAGE(STATUS "other platform")
endif ()
