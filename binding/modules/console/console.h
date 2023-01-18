#ifndef BINDING_MODULES_CONSOLE_H_
#define BINDING_MODULES_CONSOLE_H_

#include "v8-forward.h"
#include "core/js_isolate.h"
#include "core/js_context.h"

namespace bind {

class Console {
 public:
  enum ConsoleMessageLevel {
    CONSOLE_MESSAGE_LEVEL_VERBOSE,
    CONSOLE_MESSAGE_LEVEL_WARNING,
    CONSOLE_MESSAGE_LEVEL_ERROR,
    CONSOLE_MESSAGE_LEVEL_FATAL,
    CONSOLE_MESSAGE_LEVEL_LAST = CONSOLE_MESSAGE_LEVEL_FATAL
  };
  static void Register(nica::JSIsolate* js_isolate, nica::JSContext* js_context);
};

}  // namespace bind

#endif  // BINDING_MODULES_CONSOLE_H_
