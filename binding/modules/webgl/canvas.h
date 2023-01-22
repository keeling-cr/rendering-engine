#ifndef BINDING_MODULES_CANVAS_H_
#define BINDING_MODULES_CANVAS_H_


#include "core/converter.h"
#include "v8-template.h"
#include "core/js_isolate.h"
#include "core/js_context.h"

namespace bind {

class Canvas {
  public:
    Canvas(int width, int height);
    ~Canvas();
    static void Register(nica::JSIsolate* js_isolate, nica::JSContext* js_context);
    void SetWidth(int width) { width_ = width; }
    void SetHeight(int height) { height_ = height; } 
    int width() { return width_; }
    int height() { return height_; }
  private:

    int width_;
    int height_;
};

} // namespace bind

#endif  // BINDING_MODULES_CANVAS_H_