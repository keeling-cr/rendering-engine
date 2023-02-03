#ifndef BINDING_MODULES_WEBGL_WEBGL_PROGRAM_H
#define BINDING_MODULES_WEBGL_WEBGL_PROGRAM_H

#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/wrapper_info.h"

namespace v8 {
class Isolate;
}; // namespace v8

namespace bind {

class WebGLProgram : public WebGLObject<WebGLProgram, GLuint> {
 public:
  static const char* const ClassName() { return "WebGLProgram"; }
  static nica::WrapperInfo kWrapperInfo;
  static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
 protected:
  WebGLProgram(v8::Isolate* isolate, WebGLRenderingContext* context, GLuint program_id)
      : WebGLObject<WebGLProgram, GLuint>(isolate, context, program_id) {}

  friend class WebGLRenderingContext;
};

}

#endif // BINDING_MODULES_WEBGL_WEBGL_PROGRAM_H
