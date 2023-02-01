#ifndef BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_
#define BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_


#include "core/arguments.h"
#include "core/wrappable.h"
#include "core/function_template_builder.h"
#include "core/converter.h"
#include "core/js_isolate.h"
#include "core/js_context.h"
#include "v8-template.h"
#include "core/v8_object.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

namespace bind {

class WebGLBuffer;

class WebGLRenderingContext : public nica::V8Object<WebGLRenderingContext> {
  public:
    static unsigned long s_context_counter;
    
    WebGLRenderingContext(v8::Isolate* isolate);
    ~WebGLRenderingContext();

    static nica::WrapperInfo kWrapperInfo;
    static const char* ClassName() { return "WebGLRenderingContext"; }
    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
    
    unsigned long context_id() { return context_id_; }

    void ClearColor(float read, float green, float blue, float alpha);
    WebGLBuffer* CreateBuffer();
    void BindBuffer(WebGLBuffer* buffer);
  private:
    bool InitGlfw();

    GLFWwindow* window_ = nullptr;    
    unsigned long context_id_;
};

} // namespace bind

#endif  // BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_