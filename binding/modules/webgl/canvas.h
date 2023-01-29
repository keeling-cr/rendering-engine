#ifndef BINDING_MODULES_CANVAS_H_
#define BINDING_MODULES_CANVAS_H_

#include "core/arguments.h"
#include "core/function_template_builder.h"
#include "core/wrappable.h"
#include "core/converter.h"
#include "v8-template.h"
#include "core/js_isolate.h"
#include "core/js_context.h"

namespace bind {

class Canvas : public nica::Wrappable<Canvas>{
  public:
    static nica::WrapperInfo kWrapperInfo;
    static const bool is_dynamic_obj = true;

    Canvas(int width, int height);
    ~Canvas();
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;
  
    static void Register(
      nica::JSIsolate* js_isolate, nica::JSContext* js_context);

  private:
    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);

    const char* GetTypeName() final { return "Canvas"; }

    void SetWidth(int width) { width_ = width; }
    void SetHeight(int height) { height_ = height; } 
    int width() { return width_; }
    int height() { return height_; }

    int width_;
    int height_;
};

} // namespace bind

#endif  // BINDING_MODULES_CANVAS_H_