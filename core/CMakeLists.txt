set(CORE_SOURCE 
    js_isolate.cc
    js_runtime.cc
    js_context.cc
    converter.cc)

add_library(libcore ${CORE_SOURCE})