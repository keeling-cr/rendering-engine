#include <cstdio>
#include <string>

#include "base/files/file_util.h"
#include "base/logging.h"
#include "core/js_runtime.h"
#include "core/js_isolate.h"
#include "core/js_context.h"
#include "core/converter.h"
#include "core/try_catch.h"
#include "binding/modules/console/console.h"
#include "binding/modules/webgl/canvas.h"
#include "binding/modules/test/bind_test.h"

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
    InstallBuiltinModule();
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
    v8::Isolate* isolate = js_isolate_->isolate(); 

    v8::Local<v8::Context> context =
      v8::Context::New(isolate);
    js_context_ = JSContext::Create(isolate);
    js_context_->SetContext(context);
}

void JSRuntime::InstallBuiltinModule() {
    bind::Console::Register(js_isolate_.get(), js_context_.get());
    bind::Canvas::Register(js_isolate_.get(), js_context_.get());
    bind::BindTest::Register(js_isolate_.get(), js_context_.get());
}
    
void JSRuntime::EvaluateJavascriptSource(const std::string& source) {
    v8::Isolate* isolate = js_isolate_->isolate();
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
    if (ctx.IsEmpty()) 
        LOG(ERROR) << "keiling current context is empty";

    v8::Local<v8::Context> context = js_context_->context();
    TryCatch try_catch(isolate);

    auto maybe_script =
        v8::Script::Compile(
            context, StringToV8(isolate, source));
    
    v8::Local<v8::Script> script;
    if (!maybe_script.ToLocal(&script)) {
        CHECK(false) << try_catch.GetStackTrace();
        return;
    }

    auto maybe = script->Run(context);
    v8::Local<v8::Value> result;
    if (!maybe.ToLocal(&result)) {
        CHECK(false) << try_catch.GetStackTrace();
        return;
    }
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