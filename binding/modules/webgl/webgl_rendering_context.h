#ifndef BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_
#define BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_


#include "core/arguments.h"
#include "core/function_template_builder.h"
#include "core/converter.h"
#include "core/typed_array/array_buffer.h"
#include "core/v8_object.h"
#include "core/script_value.h"
#include "angle/gles_loader_autogen.h"

namespace bind {

class WebGLBuffer;
class WebGLShader;
class WebGLProgram;
class WebGLTexture;
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

    void BufferData(GLenum target, nica::ArrayBufferView* buffer, GLenum usage);
    void ClearColor(float read, float green, float blue, float alpha);
    WebGLBuffer* CreateBuffer();
    void BindBuffer(GLenum target, WebGLBuffer* buffer);
    WebGLShader* CreateShader(GLenum type);
    void ShaderSource(WebGLShader* shader, const std::string& source);
    void CompileShader(WebGLShader* shader);

    WebGLProgram* CreateProgram();
    void LinkProgram(WebGLProgram* program);
    void UseProgram(WebGLProgram* program);
    void AttachShader(WebGLProgram* program, WebGLShader* shader);

    GLint GetAttribLocation(WebGLProgram* program, const std::string& name);
    void VertexAttribPointer(
      GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
    void EnableVertexAttribArray(GLuint index);

    void ActiveTexture(GLenum texture);
    void BindTexture(GLenum target, WebGLTexture* texture);
    WebGLTexture* CreateTexture();
    void DeleteTexture(WebGLTexture* texture);
    nica::ScriptValue GetTexParameter(GLenum target, GLenum pname);
    GLboolean IsTexture(WebGLTexture* texture);

    void Clear(GLbitfield mask);
    void Enable(GLenum cap);
    void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void DrawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset);
    void DrawArrays(GLenum mode, GLint first, GLsizei count);
  private:
    bool ValidateTextureBinding(const char* function, GLenum target, bool use_six_enums);
    bool ValidateCapability(const char* function, GLenum cap);
    bool ValidateObject(WebGLObjectInterface* object);
    bool ValidateBufferDataParameters(const char* function, GLenum target, GLenum usage);
    bool RequireObject(const void* object);


    void DeleteBufferInMap(WebGLBuffer* buffer);
    void DeleteShaderInMap(WebGLShader* shader);
    void DeleteProgramInMap(WebGLShader* program);
    void DeleteTextureInMap(WebGLTexture* texture);

    template<class T>
    void DeleteMapObjects(std::map<GLuint, T*>& map) {
      typename std::map<GLuint, T*>::iterator it;
      for (it = map.begin(); it != map.end(); it++)
        delete it->second;
    }

    void set_gl_error(GLenum error);
    GLenum gl_error();

    unsigned long context_id_;
    GLenum gl_error_;
    std::map<GLuint, WebGLBuffer*> buffer_map_;
    std::map<GLuint, WebGLShader*> shader_map_;
    std::map<GLuint, WebGLProgram*> program_map_;
    std::map<GLuint, WebGLTexture*> texture_map_;
};

} // namespace bind

#endif  // BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_