#include <cstdio>
#include <string>

#include "base/files/file_util.h"
#include "base/logging.h"
#include "core/js_runtime.h"
#include "core/js_isolate.h"
#include "core/js_context.h"
#include "core/converter.h"

namespace nica {

JSRuntime::Scope::Scope(JSRuntime* runtime)
    : isolate_scope_(runtime->GetJSIsolate()->isolate()),
      handle_scope_(runtime->GetJSIsolate()->isolate()),
      scope_(runtime->GetJSContext()->context()) {
}

JSRuntime::Scope::~Scope() = default;

JSRuntime::JSRuntime() {
    platform_ = v8::platform::NewDefaultPlatform(); 
    v8::V8::InitializePlatform(platform_.get());
    v8::V8::Initialize();
    js_isolate_  = JSIsolate::Create();
    CreateJSContext();
}

JSRuntime::~JSRuntime() {
    v8::V8::Dispose();
}

JSContext* JSRuntime::GetJSContext() {
    return js_context_.get();
}

JSIsolate* JSRuntime::GetJSIsolate() {
    return js_isolate_.get();
}

void JSRuntime::CreateJSContext() {
    v8::Isolate::Scope isolate_scope(js_isolate_->isolate());
    v8::HandleScope handle_scope(js_isolate_->isolate());
    v8::Local<v8::Context> context =
      v8::Context::New(js_isolate_->isolate());

    js_context_ = JSContext::Create(js_isolate_->isolate());
    js_context_->SetContext(context);
}
    
void JSRuntime::EvaluateJavascriptSource(const std::string& source) {
    v8::Isolate* isolate = js_isolate_->isolate();
    v8::Local<v8::Context> context = js_context_->context();
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Script> script =
        v8::Script::Compile(
            context, StringToV8(isolate, source)).ToLocalChecked();
    // Run the script to get the result.
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    // Convert the result to an UTF8 string and print it.
    v8::String::Utf8Value utf8(isolate, result);
    std::cout << *utf8 << std::endl;
}

void JSRuntime::EvaluateJavascriptFile(const base::FilePath& js_file_path) {
    std::string source;
    if (!base::ReadFileToString(js_file_path, &source)) {
        LOG(FATAL) << "Unable to read " << js_file_path.value();
        return;
    }
    EvaluateJavascriptSource(source);
}

} // namespace Nica