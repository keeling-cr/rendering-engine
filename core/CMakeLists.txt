set(CORE_SOURCE 
    typed_array/array_buffer_contents.cc
    arguments.cc
    js_isolate.cc
    js_runtime.cc
    js_context.cc
    converter.cc
    try_catch.cc
    wrapper_info.cc
    object_template_builder.cc
    function_template.cc
    per_isolate_data.cc
    interceptor.cc
    wrappable.cc
    function_template_builder.cc
    v8_object.cc)

add_library(libcore ${CORE_SOURCE})