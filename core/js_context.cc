#include <memory>

#include "js_context.h"

namespace nica {

JSContext::JSContext(v8::Isolate* isolate)
    : isolate_(isolate) {}

JSContext::~JSContext() {}

std::unique_ptr<JSContext> JSContext::Create(v8::Isolate* isolate) {
    return std::unique_ptr<JSContext>(new JSContext(isolate));
}

void JSContext::SetContext(v8::Local<v8::Context> context) {
    context_.Reset(isolate_, context);
    context_.AnnotateStrongRetainer("nica::JSContext::context_");
}

} // namespace nica