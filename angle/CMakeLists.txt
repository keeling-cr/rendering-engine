find_library(COCOA_LIBRARY Cocoa)

list(APPEND ANGLE_LIBRARIES
    ${COCOA_LIBRARY}
)

set(ANGLE_UTIL_SOURCE 
    egl_window.cc
    egl_loader_autogen.cc
    os_window.cc
    platform/osx/osx_window.mm)


add_library(libangleutil ${ANGLE_UTIL_SOURCE})

target_link_libraries(libangleutil PUBLIC ${ANGLE_LIBRARIES})