
#include "binding/modules/console/console.h"

#include <stdio.h>

#include "base/logging.h"
#include "base/strings/string_util.h"
#include "core/arguments.h"
#include "core/converter.h"
#include "core/embedders.h"
#include "core/wrapper_info.h"
#include "v8-template.h"

namespace bind {

namespace {

void BoundLogMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  std::string message;
  for (int i = 0; i < info.Length(); ++i) {
    if (i > 0)
      message += " ";
    message += *v8::String::Utf8Value(info.GetIsolate(), info[i]);
  }

  const auto level = static_cast<Console::ConsoleMessageLevel>(
      info.Data().As<v8::Int32>()->Value());

  switch (level) {
    case Console::ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_VERBOSE:
      LOG(INFO) << "nica console message: " << message;
      break;
    case Console::ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_FATAL:
      LOG(FATAL) << "nica console message: " << message;
      break;
    case Console::ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_WARNING:
      LOG(WARNING) << "nica console message: " << message;
      break;
    case Console::ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_ERROR:
      LOG(ERROR) << "nica console message: " << message;
      break;
  }
}

}  // namespace

void Console::Register(nica::JSIsolate* js_isolate, nica::JSContext* js_context) {
  v8::Isolate* isolate = js_isolate->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = js_context->context();
  v8::Context::Scope context_scope(context);

  v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);

  static const struct {
    const char* name;
    ConsoleMessageLevel level;
  } methods[] = {
      {"debug", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_VERBOSE},
      {"log", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_VERBOSE},
      {"warn", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_WARNING},
      {"error", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_ERROR},
      {"fatal", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_FATAL},
  };

  for (const auto& method : methods) {
    v8::Local<v8::FunctionTemplate> function = v8::FunctionTemplate::New(
        isolate, BoundLogMethodCallback,
        v8::Integer::New(isolate, static_cast<int>(method.level)),
        v8::Local<v8::Signature>(), 0, v8::ConstructorBehavior::kThrow);
    templ->Set(nica::StringToSymbol(isolate, method.name), function);
  }

  context->Global()
    ->Set(context, v8::String::NewFromUtf8Literal(isolate, "console"),
          templ->NewInstance(isolate->GetCurrentContext()).ToLocalChecked())
    .FromJust();
}

}  // namespace bind
