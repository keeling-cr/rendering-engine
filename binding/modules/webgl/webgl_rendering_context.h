#ifndef BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_
#define BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_


#include "core/arguments.h"
#include "core/wrappable.h"
#include "core/function_template_builder.h"
#include "core/converter.h"
#include "core/js_isolate.h"
#include "core/js_context.h"
#include "v8-template.h"
#include "core/v8_object.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

namespace bind {

class WebGLBuffer;
class WebGLShader;
class WebGLObjectInterface;

class WebGLRenderingContext : public nica::V8Object<WebGLRenderingContext> {
  public:
    static unsigned long s_context_counter;
    
    WebGLRenderingContext(v8::Isolate* isolate);
    ~WebGLRenderingContext();

    static nica::WrapperInfo kWrapperInfo;
    static const char* ClassName() { return "WebGLRenderingContext"; }
    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
      v8::Isolate* isolate);
    
    unsigned long context_id() { return context_id_; }

    void ClearColor(float read, float green, float blue, float alpha);
    WebGLBuffer* CreateBuffer();
    void BindBuffer(GLenum target, WebGLBuffer* buffer);
    WebGLShader* CreateShader(GLenum type);
  private:
    bool InitGlfw();
    bool ValidateObject(WebGLObjectInterface* object);

    template<class T>
    void DeleteMapObjects(std::map<GLuint, T*>& map) {
      typename std::map<GLuint, T*>::iterator it;
      for (it = map.begin(); it != map.end(); it++)
        delete it->second;
    }

    void set_gl_error(GLenum error);
    GLenum gl_error();

    GLFWwindow* window_ = nullptr;    
    unsigned long context_id_;
    GLenum gl_error_;
    // todo(liqining): memory release
    std::map<GLuint, WebGLBuffer*> buffer_map_;
    std::map<GLuint, WebGLShader*> shader_map_;
};

} // namespace bind

#endif  // BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_