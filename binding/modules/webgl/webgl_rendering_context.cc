#include "binding/modules/webgl/webgl_rendering_context.h"
#include "base/logging.h"

namespace bind {

unsigned long WebGLRenderingContext::s_context_counter = 0;

nica::WrapperInfo WebGLRenderingContext::kWrapperInfo = {
    nica::kEmbedderNicaMain};

WebGLRenderingContext:: WebGLRenderingContext(v8::Isolate* isolate)
    : V8Object(isolate)
    , context_id_(s_context_counter++) {}

WebGLRenderingContext::~WebGLRenderingContext() = default;

nica::FunctionTemplateBuilder 
WebGLRenderingContext::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    return nica::FunctionTemplateBuilder(isolate, nullptr)
        .SetMethod("test", std::function<void(WebGLRenderingContext*)>{&WebGLRenderingContext::test});
}

} // namespace bind
