#ifndef BINDING_MODULES_WEBGL_OBJECT_H_
#define BINDING_MODULES_WEBGL_OBJECT_H_

#include "v8-forward.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "core/v8_object.h"

namespace bind {

class WebGLObjectInterface {
 public:
  virtual ~WebGLObjectInterface() {}
  virtual bool ValidateContext(WebGLRenderingContext* context) = 0;
};

template<class V, typename T>
class WebGLObject : public nica::V8Object<V>, public WebGLObjectInterface {
 public:
  WebGLObject(v8::Isolate* isolate, WebGLRenderingContext* context, T webgl_id)
      : nica::V8Object<V>(isolate)
      , context_id_(context->context_id())
      , webgl_id_(webgl_id) {}

  bool ValidateContext(WebGLRenderingContext* context) { return context->context_id() == context_id_; }
  T webgl_id() { return webgl_id_; }

 private:
  unsigned long context_id_;
  T webgl_id_;
};

}

#endif
