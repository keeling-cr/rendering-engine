#ifndef BINDING_MODULES_WEBGL_WEBGL_TEXTURE_H
#define BINDING_MODULES_WEBGL_WEBGL_TEXTURE_H

#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/wrapper_info.h"

namespace v8 {
class Isolate;
}; // namespace v8

namespace bind {

class WebGLTexture : public WebGLObject<WebGLTexture, GLuint> {
 public:
  static const char* const ClassName() { return "WebGLTexture"; }
  static nica::WrapperInfo kWrapperInfo;
  static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
 protected:
  WebGLTexture(v8::Isolate* isolate, WebGLRenderingContext* context, GLuint texture_id)
      : WebGLObject<WebGLTexture, GLuint>(isolate, context, texture_id) {}

friend class WebGLRenderingContext;
};

}

#endif // BINDING_MODULES_WEBGL_WEBGL_TEXTURE_H
