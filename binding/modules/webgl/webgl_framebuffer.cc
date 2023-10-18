#include "binding/modules/webgl/webgl_framebuffer.h"

namespace bind {

nica::WrapperInfo WebGLFramebuffer::kWrapperInfo = {
    nica::kEmbedderNicaMain};

nica::FunctionTemplateBuilder 
WebGLFramebuffer::GetFunctionTemplateBuilder(v8::Isolate* isolate) {
  nica::FunctionTemplateBuilder builder(isolate, nullptr);
  return builder;
}

} // namespace bind