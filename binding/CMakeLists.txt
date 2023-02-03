set(BIND_SOURCE 
    modules/webgl/glad.c
    modules/console/console.cc
    modules/webgl/canvas.cc
    modules/test/bind_test.cc
    modules/test/dynamic_bind_test.cc
    modules/webgl/webgl_buffer.cc
    modules/webgl/webgl_rendering_context.cc
    modules/webgl/webgl_shader.cc
    modules/webgl/webgl_program.cc)

add_library(libv8bind ${BIND_SOURCE})