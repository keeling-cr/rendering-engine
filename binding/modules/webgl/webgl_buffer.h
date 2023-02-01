#ifndef BINDING_MODULES_WEBGL_WEBGL_BUFFER_H
#define BINDING_MODULES_WEBGL_WEBGL_BUFFER_H

#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/wrapper_info.h"

namespace v8 {
class Isolate;
} // v8

namespace bind {

class WebGLBuffer : public WebGLObject<WebGLBuffer, GLuint> {
 public:
  static const char* ClassName() { return "WebGLBuffer"; }
  static nica::WrapperInfo kWrapperInfo;
  static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
 protected:
  WebGLBuffer(v8::Isolate* isolate, WebGLRenderingContext* context, GLuint buffer_id)
      : WebGLObject<WebGLBuffer, GLuint>(isolate, context, buffer_id) {}

  friend class WebGLRenderingContext;
};

} // namespace bind

#endif // BINDING_MODULES_WEBGL_WEBGL_BUFFER_H
