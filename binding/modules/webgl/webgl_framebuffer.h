#ifndef BINDING_MODULES_WEBGL_FRAMEBUFFER_H
#define BINDING_MODULES_WEBGL_FRAMEBUFFER_H

#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/wrapper_info.h"

namespace v8 {
class Isolate;
} // v8

namespace bind {

class WebGLFramebuffer : public WebGLObject<WebGLFramebuffer, GLuint> {
 public:
  static const char* ClassName() { return "WebGLFramebuffer"; }
  static nica::WrapperInfo kWrapperInfo;
  static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
 protected:
  WebGLFramebuffer(v8::Isolate* isolate, WebGLRenderingContext* context, GLuint renderbuffer_id)
      : WebGLObject<WebGLFramebuffer, GLuint>(isolate, context, renderbuffer_id) {}

  friend class WebGLRenderingContext;
};

} // namespace bind

#endif // BINDING_MODULES_WEBGL_FRAMEBUFFER_H
