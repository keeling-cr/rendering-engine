#include "binding/modules/webgl/webgl_shader.h"

namespace bind {

nica::WrapperInfo WebGLShader::kWrapperInfo = {
    nica::kEmbedderNicaMain};

nica::FunctionTemplateBuilder 
WebGLShader::GetFunctionTemplateBuilder(v8::Isolate* isolate) {
  nica::FunctionTemplateBuilder builder(isolate, nullptr);
  return builder;
}

} // namespace bind