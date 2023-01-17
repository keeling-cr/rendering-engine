
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
  LOG(ERROR) << "nica console message: " << message;
}

nica::WrapperInfo kWrapperInfo = {nica::kEmbedderNicaMain};

}  // namespace

v8::Local<v8::Object> Console::AsV8Object(v8::Isolate* isolate) {
  v8::EscapableHandleScope handle_scope(isolate);

  v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);

  static const struct {
    const char* name;
    ConsoleMessageLevel level;
  } methods[] = {
      {"debug", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_VERBOSE},
      {"log", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_INFO},
      {"warn", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_WARNING},
      {"error", ConsoleMessageLevel::CONSOLE_MESSAGE_LEVEL_ERROR},
  };

  for (const auto& method : methods) {
    v8::Local<v8::FunctionTemplate> function = v8::FunctionTemplate::New(
        isolate, BoundLogMethodCallback,
        v8::Integer::New(isolate, static_cast<int>(method.level)),
        v8::Local<v8::Signature>(), 0, v8::ConstructorBehavior::kThrow);
    templ->Set(nica::StringToSymbol(isolate, method.name), function);
  }
  v8::Local<v8::Context> ctx = isolate->GetCurrentContext();
  return handle_scope.Escape(
      templ->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

}  // namespace bind
