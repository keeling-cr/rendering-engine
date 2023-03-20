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
class WebGLFramebuffer;
class WebGLShader;
class WebGLProgram;
class WebGLRenderbuffer;
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

    void BindRenderbuffer(GLenum target, WebGLRenderbuffer* renderbuffer);
    WebGLRenderbuffer* CreateRenderbuffer();
    void DeleteRenderbuffer(WebGLRenderbuffer* renderbuffer);
    nica::ScriptValue GetRenderbufferParameter(GLenum target, GLenum pname);
    bool IsRenderbuffer(WebGLRenderbuffer* renderbuffer);
    void RenderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height);

    void BindFramebuffer(GLenum target, WebGLFramebuffer* framebuffer);
    GLenum CheckFramebufferStatus(GLenum target);
    WebGLFramebuffer* CreateFramebuffer();
    void DeleteFramebuffer(WebGLFramebuffer* framebuffer);
    // nica::ScriptValue GetFramebufferAttachmentParameter(GLenum target, GLenum attachment, GLenum pname);

    void BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void BlendEquation(GLenum mode);
    void BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
    void BlendFunc(GLenum sfactor, GLenum dfactor);
    void BlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

    void StencilFunc(GLenum func, GLint ref, GLuint mask);
    void StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
    void StencilMask(GLuint mask);
    void StencilMaskSeparate(GLenum face, GLuint mask);
    void StencilOp(GLenum fail, GLenum zfail, GLenum zpass);
    void StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);

    void Clear(GLbitfield mask);
    void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void ClearDepth(GLclampf depth);
    void ClearStencil(GLint s);
    
    void Enable(GLenum cap);
    void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void DrawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset);
    void DrawArrays(GLenum mode, GLint first, GLsizei count);
  private:
    bool ValidateStencilFunc(const char* function, GLenum func);
    bool ValidateBlendFuncFactors(const char* function, GLenum src, GLenum dst);
    bool ValidateBlendEquation(const char* function, GLenum mode);
    bool ValidateTextureBinding(const char* function, GLenum target, bool use_six_enums);
    bool ValidateCapability(const char* function, GLenum cap);
    bool ValidateObject(WebGLObjectInterface* object);
    bool ValidateBufferDataParameters(const char* function, GLenum target, GLenum usage);
    bool RequireObject(const void* object);

    void DeleteBufferInMap(WebGLBuffer* buffer);
    void DeleteShaderInMap(WebGLShader* shader);
    void DeleteProgramInMap(WebGLShader* program);
    void DeleteTextureInMap(WebGLTexture* texture);
    void DeleteRenderbufferInMap(WebGLRenderbuffer* renderbuffer);
    void DeleteFramebufferInMap(WebGLFramebuffer* framebuffer);

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
    std::map<GLuint, WebGLRenderbuffer*> renderbuffer_map_;
    std::map<GLuint, WebGLFramebuffer*> framebuffer_map_;
};

} // namespace bind

#endif  // BINDING_MODULES_WEBGL_RENDERING_CONTEXT_H_