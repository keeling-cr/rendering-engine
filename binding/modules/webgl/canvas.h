#ifndef BINDING_MODULES_CANVAS_H_
#define BINDING_MODULES_CANVAS_H_

#include "angle/egl_window.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/arguments.h"
#include "core/function_template_builder.h"
#include "core/wrappable.h"
#include "core/converter.h"
#include "core/v8_object.h"
#include "v8-template.h"
#include "platform/window/osx/osx_window.h"

namespace bind {

class Canvas : public nica::V8Object<Canvas>{
  public:
    Canvas(v8::Isolate* isolate, v8::Local<v8::Object> instance,
      int width, int height);
    ~Canvas();
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    static nica::WrapperInfo kWrapperInfo;
    static const char* ClassName() { return "Canvas"; }
    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
  
  private:

    void SetWidth(int width) { width_ = width; }
    void SetHeight(int height) { height_ = height; } 
    int width() { return width_; }
    int height() { return height_; }
    WebGLRenderingContext* GetContext();
    void SwapBufferForTest();

    int width_;
    int height_;
    
    std::unique_ptr<angle::EGLWindow> egl_window_;
    std::unique_ptr<WebGLRenderingContext> webgl_rendering_context_;
};

} // namespace bind

#endif  // BINDING_MODULES_CANVAS_H_