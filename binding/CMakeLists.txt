set(BIND_SOURCE 
    modules/console/console.cc
    modules/webgl/canvas.cc
    modules/test/bind_test.cc
    modules/test/dynamic_bind_test.cc
    modules/webgl/webgl_rendering_context.cc)

add_library(libv8bind ${BIND_SOURCE})