
include_guard(GLOBAL)

include(CMakeParseArguments)

if(WIN32)
    list(APPEND _ENV DEPOT_TOOLS_WIN_TOOLCHAIN=0)
endif()

list(APPEND _THIRDPARTY_PATH ${DEPOT_TOOLS_PATH}:$ENV{PATH})
list(APPEND _ENV PATH="${_THIRDPARTY_PATH}")
set(PREFIX_EXECUTE ${CMAKE_COMMAND} -E env ${_ENV})

message(WARNING "prefix-exe" ${PREFIX_EXECUTE})
function(thirdparty_command)
    set(ONE_VALUE_ARGS NAME COMMENT WORKING_DIRECTORY)
    set(LIST_ARGS COMMAND DEPENDS)
    cmake_parse_arguments(COMMAND "" "${ONE_VALUE_ARGS}" "${LIST_ARGS}" ${ARGN})

    set(STAMP_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
    set(STAMP_FILE ${STAMP_DIR}/${COMMAND_NAME}-complete)

    if(WIN32)
        set(COMMAND_COMMAND cmd /c ${COMMAND_COMMAND})
    else()
        if(NOT COMMAND_COMMAND)
            set(COMMAND_COMMAND echo 1 &>/dev/null)
        endif()
    endif()

    add_custom_command(
        OUTPUT ${STAMP_FILE}
        COMMENT ${COMMAND_COMMENT}
        COMMAND ${PREFIX_EXECUTE} ${COMMAND_COMMAND}
        COMMAND ${CMAKE_COMMAND} -E touch ${STAMP_FILE}
        WORKING_DIRECTORY ${COMMAND_WORKING_DIRECTORY}
    )

    add_custom_target(${COMMAND_NAME} ALL DEPENDS ${STAMP_FILE})

    if (COMMAND_DEPENDS)
        add_dependencies(${COMMAND_NAME} ${COMMAND_DEPENDS})
    endif (COMMAND_DEPENDS)
endfunction()
