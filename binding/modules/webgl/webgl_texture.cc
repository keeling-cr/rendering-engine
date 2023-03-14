#include "binding/modules/webgl/webgl_texture.h"

namespace bind {

nica::WrapperInfo WebGLTexture::kWrapperInfo = {
    nica::kEmbedderNicaMain};

nica::FunctionTemplateBuilder 
WebGLTexture::GetFunctionTemplateBuilder(v8::Isolate* isolate) {
  nica::FunctionTemplateBuilder builder(isolate, nullptr);
  return builder;
}

} // namespace bind