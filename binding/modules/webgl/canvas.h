#ifndef BINDING_MODULES_CANVAS_H_
#define BINDING_MODULES_CANVAS_H_


#include "core/converter.h"
#include "v8-template.h"
#include "core/js_isolate.h"
#include "core/js_context.h"

namespace bind {

class Canvas {
 public:
  Canvas(int x, int y);
  ~Canvas();
  static void Register(nica::JSIsolate* js_isolate, nica::JSContext* js_context);

    int x_, y_;
};

} // namespace bind

#endif  // BINDING_MODULES_CANVAS_H_