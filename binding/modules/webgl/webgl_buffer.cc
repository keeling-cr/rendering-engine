#include "binding/modules/webgl/webgl_buffer.h"

namespace bind {

nica::WrapperInfo WebGLBuffer::kWrapperInfo = {
    nica::kEmbedderNicaMain};

nica::FunctionTemplateBuilder 
WebGLBuffer::GetFunctionTemplateBuilder(v8::Isolate* isolate) {
  nica::FunctionTemplateBuilder builder(isolate, nullptr);
  return builder;
}

} // namespace bind