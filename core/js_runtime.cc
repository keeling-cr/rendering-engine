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
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "binding/modules/webgl/webgl_buffer.h"
#include "binding/modules/webgl/webgl_renderbuffer.h"
#include "binding/modules/webgl/webgl_framebuffer.h"
#include "binding/modules/webgl/webgl_shader.h"
#include "binding/modules/webgl/webgl_program.h"

namespace nica {

JSRuntime::Scope::Scope(JSRuntime* runtime)
    : isolate_scope_(runtime->GetJSIsolate()->isolate()),
      handle_scope_(runtime->GetJSIsolate()->isolate()),
      scope_(runtime->GetJSContext()->context()) {
    runtime->InstallBuiltinModule();
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
    v8::Isolate* isolate = js_isolate_->isolate(); 

    v8::Local<v8::Context> context =
      v8::Context::New(isolate);
    js_context_ = JSContext::Create(isolate);
    js_context_->SetContext(context);
}

void JSRuntime::InstallBuiltinModule() {
#define REGISTER(module) bind::module::Register(js_isolate_.get(),js_context_.get())
    REGISTER(Console);
    REGISTER(Canvas);
    REGISTER(WebGLRenderingContext);
    REGISTER(WebGLBuffer);
    REGISTER(WebGLShader);
    REGISTER(WebGLProgram);
    REGISTER(WebGLRenderbuffer);
    REGISTER(WebGLFramebuffer);
#undef REGISTER
}
    
void JSRuntime::EvaluateJavascriptSource(const std::string& source) {
    v8::Isolate* isolate = js_isolate_->isolate();
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
    if (ctx.IsEmpty()) 
        LOG(ERROR) << "Current context is empty!!!";

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