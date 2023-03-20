set(BIND_SOURCE 
    modules/console/console.cc
    modules/webgl/canvas.cc
    modules/webgl/webgl_any.cc
    modules/webgl/webgl_buffer.cc
    modules/webgl/webgl_program.cc
    modules/webgl/webgl_rendering_context.cc
    modules/webgl/webgl_renderbuffer.cc
    modules/webgl/webgl_shader.cc
    modules/webgl/webgl_texture.cc)

add_library(libv8bind ${BIND_SOURCE})