#include "binding/modules/webgl/webgl_rendering_context.h"
#include "base/logging.h"

namespace bind {

nica::WrapperInfo WebGLRenderingContext::kWrapperInfo = {
    nica::kEmbedderNicaMain};

WebGLRenderingContext::WebGLRenderingContext(
    v8::Isolate* isolate,
    v8::Local<v8::Object> instance)
    : V8Object(isolate, instance) {}

WebGLRenderingContext:: WebGLRenderingContext(v8::Isolate* isolate)
    : V8Object(isolate) {}

WebGLRenderingContext::~WebGLRenderingContext() = default;

nica::FunctionTemplateBuilder 
WebGLRenderingContext::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    return nica::FunctionTemplateBuilder(isolate, nullptr)
        .SetMethod("test", std::function<void(WebGLRenderingContext*)>{&WebGLRenderingContext::test});
}

} // namespace bind
