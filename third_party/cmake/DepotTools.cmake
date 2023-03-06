ExternalProject_Add(depot-tools
    GIT_REPOSITORY    https://chromium.googlesource.com/chromium/tools/depot_tools.git
    GIT_TAG           main
    PREFIX            ${CMAKE_BINARY_DIR}/depot_tools
    SOURCE_DIR        ${CMAKE_SOURCE_DIR}/depot_tools
    UPDATE_COMMAND    ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
)

ExternalProject_Get_Property(depot-tools SOURCE_DIR)
message(WARNING "depot tools source dir in depottools" ${SOURCE_DIR})
set(DEPOT_TOOLS_PATH ${SOURCE_DIR})