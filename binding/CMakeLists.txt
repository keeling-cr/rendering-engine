set(BIND_SOURCE 
    modules/console/console.cc
    modules/webgl/canvas.cc)

add_library(libv8bind ${BIND_SOURCE})