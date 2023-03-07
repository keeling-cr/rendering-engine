cmake_minimum_required(VERSION 3.8)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gen)

message(WARNING "build type " ${BUILD_TYPE})

include(DepotTools)
include(ThirdPartyCommand)
include(Angle)
include(V8)
