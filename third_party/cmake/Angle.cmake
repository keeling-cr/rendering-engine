set(ANGLE_FOLDER ${CMAKE_SOURCE_DIR}/angle)
set(ANGLE_OUTPUT_FOLDER ${CMAKE_SOURCE_DIR}/gen/angle)

set(DEPOT_TOOLS_CONFIG_COMMAND git config core.autocrlf false)
thirdparty_command(
    NAME depot-tools-config
    COMMAND ${DEPOT_TOOLS_CONFIG_COMMAND}
    WORKING_DIRECTORY ${DEPOT_TOOLS_PATH}
    DEPENDS depot-tools
)

set(ANGLE_FETCH_COMMAND fetch --no-history angle)
thirdparty_command(
    NAME fetch-angle
    COMMAND ${ANGLE_FETCH_COMMAND}
    WORKING_DIRECTORY ${ANGLE_FOLDER}
)

if (UNIX AND NOT APPLE)
    set(ANGLE_INSTALL_DEPS ./build/install-build-deps.sh)
    thirdparty_command(
        NAME install-deps
        COMMAND ${ANGLE_INSTALL_DEPS}
        WORKING_DIRECTORY ${ANGLE_FOLDER}
        DEPENDS fetch-angle
)
endif()

list(APPEND ANGLE_GEN_ARGS
    is_component_build = false
    angle_assert_always_on = false
)

message(WARNING "build type " ${BUILD_TYPE})
if(BUILD_TYPE STREQUAL "debug")
    list(APPEND ANGLE_GEN_ARGS
        is_debug = true
    )
else()
    list(APPEND ANGLE_GEN_ARGS
        is_debug = false
    )
endif()

set(ANGLE_CONFIGURE_COMMAND gn gen out/${BUILD_TYPE} --args="${ANGLE_GEN_ARGS}")
message(WARNING "angle config command" ${ANGLE_CONFIGURE_COMMAND})
thirdparty_command(
    NAME gen
    COMMAND ${ANGLE_CONFIGURE_COMMAND}
    WORKING_DIRECTORY ${ANGLE_FOLDER}
    DEPENDS fetch-angle
)

set(ANGLE_BUILD_COMMAND ninja -C out/${BUILD_TYPE})
thirdparty_command(
    NAME ninja
    COMMAND ${ANGLE_BUILD_COMMAND}
    WORKING_DIRECTORY ${ANGLE_FOLDER}
    DEPENDS gen
)


if (UNIX AND NOT APPLE)
    MESSAGE(STATUS "unix")
elseif (WIN32)
    MESSAGE(STATUS "windows")
elseif (APPLE)
    MESSAGE(WARNING "macos")
    set(ANGLE_OUTPUT_INCLUDE_COMMAND cp -r ${ANGLE_FOLDER}/include ${ANGLE_OUTPUT_FOLDER}/)
    thirdparty_command(
        NAME output_header
        COMMAND ${ANGLE_OUTPUT_INCLUDE_COMMAND}
        WORKING_DIRECTORY ${ANGLE_FOLDER}
        DEPENDS ninja
    )
    
    file(MAKE_DIRECTORY ${ANGLE_OUTPUT_FOLDER}/libs)
    list(APPEND  ANGLE_OUTPUT_LIB_COMMAND
        cp ${ANGLE_FOLDER}/out/${BUILD_TYPE}/libEGL.dylib ${ANGLE_OUTPUT_FOLDER}/libs
        && cp ${ANGLE_FOLDER}/out/${BUILD_TYPE}/libGLESv2.dylib ${ANGLE_OUTPUT_FOLDER}/libs)
    thirdparty_command(
        NAME output_libs
        COMMAND ${ANGLE_OUTPUT_LIB_COMMAND}
        WORKING_DIRECTORY ${ANGLE_FOLDER}
        DEPENDS ninja
    )
else ()
    MESSAGE(STATUS "other platform")
endif ()
