#ifndef BINDING_MODULES_WEBGL_WEBGL_SHADER_H
#define BINDING_MODULES_WEBGL_WEBGL_SHADER_H

#include <string>

#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/wrapper_info.h"

namespace v8 {
class Isolate;
} // v8


namespace bind {

class WebGLShader : public WebGLObject<WebGLShader, GLuint> {
 public:
  static const char* ClassName() { return "WebGLShader"; }
  static nica::WrapperInfo kWrapperInfo;
  static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);

  const char* source() { return source_.c_str(); }
  void set_source(const std::string& source) { source_ = source; }

  std::string log() { return log_; }
  void set_log(const std::string& log) { log_ = log; }

  bool is_valid() { return is_valid_; }
  void set_is_valid(bool valid) { is_valid_ = valid; }

 protected:
  WebGLShader(v8::Isolate* isolate, WebGLRenderingContext* context, GLuint shader_id)
      : WebGLObject<WebGLShader, GLuint>(isolate, context, shader_id)
      , is_valid_(false) {}

  friend class WebGLRenderingContext;

 private:
  bool is_valid_;
  std::string source_;
  std::string log_;
};

} // namespace bind

#endif // BINDING_MODULES_WEBGL_WEBGL_SHADER_H
