#ifndef BINDING_MODULES_CONSOLE_H_
#define BINDING_MODULES_CONSOLE_H_

#include "v8-forward.h"

namespace bind {

class Console {
 public:
  enum ConsoleMessageLevel {
    CONSOLE_MESSAGE_LEVEL_VERBOSE,
    CONSOLE_MESSAGE_LEVEL_INFO,
    CONSOLE_MESSAGE_LEVEL_WARNING,
    CONSOLE_MESSAGE_LEVEL_ERROR,
    CONSOLE_MESSAGE_LEVEL_LAST = CONSOLE_MESSAGE_LEVEL_ERROR
  };
  static void Register(v8::Isolate* isolate);
  static v8::Local<v8::Object> AsV8Object(v8::Isolate* isolate);
};

}  // namespace bind

#endif  // BINDING_MODULES_CONSOLE_H_
