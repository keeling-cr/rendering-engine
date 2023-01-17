set(CORE_SOURCE 
    arguments.cc
    js_isolate.cc
    js_runtime.cc
    js_context.cc
    converter.cc
    try_catch.cc
    wrapper_info.cc)

add_library(libcore ${CORE_SOURCE})