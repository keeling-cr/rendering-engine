#ifndef BINDING_MODULES_WEBGL_TEXTURE_H_
#define BINDING_MODULES_WEBGL_TEXTURE_H_

#include "webgl_object.h"
#include "webgl_rendering_context.h"

namespace bind {

class WebGLTexture : public WebGLObject<WebGLTexture, GLuint> {
 public:
  static const char* const ClassName() { return "WebGLTexture"; }

 protected:
  WebGLTexture(WebGLRenderingContext* context, GLuint texture_id)
      : WebGLObject<WebGLTexture, GLuint>(context, texture_id) {}

  friend class WebGLRenderingContext;
};

}

#endif // BINDING_MODULES_WEBGL_TEXTURE_H_
