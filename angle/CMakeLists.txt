find_library(COCOA_LIBRARY Cocoa)

list(APPEND ANGLE_LIBRARIES
    ${COCOA_LIBRARY}
)

set(ANGLE_UTIL_SOURCE 
    shader_util.cc
    config_params.cc
    egl_window.cc
    egl_loader_autogen.cc
    gles_loader_autogen.cc)


add_library(libangleutil ${ANGLE_UTIL_SOURCE})

target_link_libraries(libangleutil PUBLIC ${ANGLE_LIBRARIES})