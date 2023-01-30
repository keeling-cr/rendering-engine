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

namespace bind {

class WebGLRenderingContext : public nica::V8Object<WebGLRenderingContext> {
  public:
    WebGLRenderingContext(v8::Isolate* isolate, v8::Local<v8::Object> instance);
    WebGLRenderingContext(v8::Isolate* isolate);
    ~WebGLRenderingContext();

    static nica::WrapperInfo kWrapperInfo;
    static const char* ClassName() { return "WebGLRenderingContext"; }
    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
  private:
      void test() { 
        LOG(ERROR) << "keilingnica webglrendering context" << __func__;
      }
};

} // namespace bind

#endif  // BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_