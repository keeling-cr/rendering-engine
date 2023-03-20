#include "binding/modules/webgl/webgl_renderbuffer.h"

namespace bind {

nica::WrapperInfo WebGLRenderbuffer::kWrapperInfo = {
    nica::kEmbedderNicaMain};

nica::FunctionTemplateBuilder 
WebGLRenderbuffer::GetFunctionTemplateBuilder(v8::Isolate* isolate) {
  nica::FunctionTemplateBuilder builder(isolate, nullptr);
  return builder;
}

} // namespace bind