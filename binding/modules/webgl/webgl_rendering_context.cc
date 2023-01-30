#include "binding/modules/webgl/webgl_rendering_context.h"
#include "base/logging.h"

namespace bind {

namespace {
void WebGLRenderingContextNew(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    LOG(ERROR) << "keilingnica " << __func__;
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);

    if (!args.IsConstructCall()) {
        isolate->ThrowError("Canvas must be constructed with new");
        return;
    }

    // // return the new object back to the javascript caller
    args.GetReturnValue().Set(args.Holder());
}

} // namespace

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
    return nica::FunctionTemplateBuilder(isolate, WebGLRenderingContextNew)
        .SetMethod("test", std::function<void(WebGLRenderingContext*)>{&WebGLRenderingContext::test});
}

} // namespace bind
