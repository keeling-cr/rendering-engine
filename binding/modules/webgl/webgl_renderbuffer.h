#ifndef BINDING_MODULES_WEBGL_RENDERBUFFER_H
#define BINDING_MODULES_WEBGL_RENDERBUFFER_H

#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/wrapper_info.h"

namespace v8 {
class Isolate;
} // v8

namespace bind {

class WebGLRenderbuffer : public WebGLObject<WebGLRenderbuffer, GLuint> {
 public:
  static const char* ClassName() { return "WebGLRenderbuffer"; }
  static nica::WrapperInfo kWrapperInfo;
  static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
 protected:
  WebGLRenderbuffer(v8::Isolate* isolate, WebGLRenderingContext* context, GLuint renderbuffer_id)
      : WebGLObject<WebGLRenderbuffer, GLuint>(isolate, context, renderbuffer_id) {}

  friend class WebGLRenderingContext;
};

} // namespace bind

#endif // BINDING_MODULES_WEBGL_RENDERBUFFER_H
