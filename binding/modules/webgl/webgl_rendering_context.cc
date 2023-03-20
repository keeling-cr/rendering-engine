
#include "angle/shader_util.h"
#include "angle/debug_util.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "binding/modules/webgl/webgl_buffer.h"
#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_shader.h"
#include "binding/modules/webgl/webgl_program.h"
#include "binding/modules/webgl/webgl_texture.h"
#include "binding/modules/webgl/webgl_framebuffer.h"
#include "binding/modules/webgl/webgl_renderbuffer.h"
#include "binding/modules/webgl/webgl_any.h"
#include "base/logging.h"

namespace bind {

unsigned long WebGLRenderingContext::s_context_counter = 0;

nica::WrapperInfo WebGLRenderingContext::kWrapperInfo = {
    nica::kEmbedderNicaMain};

WebGLRenderingContext:: WebGLRenderingContext(v8::Isolate* isolate)
    : V8Object(isolate)
    , context_id_(s_context_counter++) {
    angle::EnableOpenGLDebug();
}

WebGLRenderingContext::~WebGLRenderingContext() {
    DeleteMapObjects(buffer_map_);
    DeleteMapObjects(shader_map_);
    DeleteMapObjects(program_map_);
    DeleteMapObjects(texture_map_);
    DeleteMapObjects(renderbuffer_map_);
}

void WebGLRenderingContext::ClearColor(
    float read, float green, float blue, float alpha) {
    glClearColor(read, green, blue, alpha);
}

WebGLBuffer* WebGLRenderingContext::CreateBuffer() {
    GLuint buffer_id = 0;
    glGenBuffers(1, &buffer_id);
    WebGLBuffer* buffer = new WebGLBuffer(GetIsolate(), this, buffer_id);
    buffer_map_[buffer_id] = buffer;
    return buffer;
}

WebGLShader* WebGLRenderingContext::CreateShader(GLenum type) {
    GLuint shader_id = glCreateShader(type);
    WebGLShader* shader = new WebGLShader(GetIsolate(), this, shader_id);
    shader_map_[shader_id] = shader;
    return shader; 
}

void WebGLRenderingContext::ShaderSource(WebGLShader* shader, const std::string& source) {
    if (!RequireObject(shader)) 
        return;
    if (!ValidateObject(shader))
        return;
    shader->set_source(source);
    const char *sourc_array[1] = {source.c_str()};
    glShaderSource(shader->webgl_id(), 1, sourc_array, nullptr);
}

void WebGLRenderingContext::CompileShader(WebGLShader* shader) {
    if (!RequireObject(shader)) 
        return;
    if (!ValidateObject(shader)) 
        return;
    
    GLuint shader_id = shader->webgl_id();
    GLint shader_type = 0;
    glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);
    if (shader_type == 0)
        return;
    
    glCompileShader(shader_id);
    GLint compile_status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

    if (!compile_status) { // If compilation was not successful
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
 
        // Get additional information
        glGetShaderInfoLog(shader_id, length, &length, message);
        LOG(ERROR) << "Cannot Compile Shader: " << message;
        shader->set_is_valid(false);
        shader->set_log(message);

        delete[] message;
        glDeleteShader(shader_id);
    }
}

WebGLProgram* WebGLRenderingContext::CreateProgram() {
    GLuint program_id = glCreateProgram();
    WebGLProgram* program = new WebGLProgram(GetIsolate(), this, program_id);
    program_map_[program_id] = program;
    return program;
}

void WebGLRenderingContext::AttachShader(
    WebGLProgram* program, WebGLShader* shader) {
    if (!RequireObject(program)) 
        return;
    if (!ValidateObject(program)) 
        return;
    GLuint program_id = program->webgl_id();

    if (!RequireObject(shader)) 
        return;
    if (!ValidateObject(shader))
        return;
    GLuint shader_id = shader->webgl_id();

    glAttachShader(program_id, shader_id);
}

void WebGLRenderingContext::LinkProgram(WebGLProgram* program) {
    if (!RequireObject(program)) 
        return;
    if (!ValidateObject(program)) 
        return;

    GLuint program_id = program->webgl_id();
    glLinkProgram(program_id);
}

void WebGLRenderingContext::UseProgram(WebGLProgram* program) {
    if (!RequireObject(program))
        return;
    if (!ValidateObject(program)) 
        return;

    GLuint program_id = program->webgl_id();
    glUseProgram(program_id);
}

GLint WebGLRenderingContext::GetAttribLocation(
    WebGLProgram* program, const std::string& name) {
    if (!RequireObject(program))
        return -1;
    if (!ValidateObject(program))
        return -1;

    GLuint program_id = program->webgl_id();
    GLint location = glGetAttribLocation(program_id, name.c_str());
    return location;
}

void WebGLRenderingContext::EnableVertexAttribArray(GLuint index) {
    glEnableVertexAttribArray(index);
}

void WebGLRenderingContext::VertexAttribPointer(
    GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset) {
    if (size < 1 || size > 4 || stride < 0 || stride > 255 || offset < 0) {
        set_gl_error(GL_INVALID_VALUE);
        return;
    }

    uint32_t type_size = 0;
    switch (type) {
        case GL_BYTE:
            type_size = sizeof(GLbyte);
            break;
        case GL_UNSIGNED_BYTE:
            type_size = sizeof(GLubyte);
            break;
        case GL_SHORT:
            type_size = sizeof(GLshort);
            break;
        case GL_UNSIGNED_SHORT:
            type_size = sizeof(GLushort);
            break;
        case GL_FLOAT:
            type_size = sizeof(GLfloat);
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return;
    }

    if ((stride % type_size) || (offset % type_size)) {
        set_gl_error(GL_INVALID_OPERATION);
        return;
    }
    glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<GLvoid*>(static_cast<intptr_t>(offset)));
}

void WebGLRenderingContext::BindBuffer(GLenum target, WebGLBuffer* buffer) {
    switch (target) {
        case GL_ARRAY_BUFFER:
        case GL_ELEMENT_ARRAY_BUFFER:
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return;
    }
    if (!ValidateObject(buffer)) 
        return;
    GLuint buffer_id = buffer ? buffer->webgl_id() : 0;
    glBindBuffer(target, buffer_id);
}

bool WebGLRenderingContext::ValidateBufferDataParameters(
    const char* function, GLenum target, GLenum usage) {
    switch (target) {
        case GL_ELEMENT_ARRAY_BUFFER:
        case GL_ARRAY_BUFFER:
            break;
        default:
            LOG(ERROR) << "keilingnica " << function << " invalid target";
            set_gl_error(GL_INVALID_ENUM);
            return false;
    }
    
    switch (usage) {
        case GL_STREAM_DRAW:
        case GL_STATIC_DRAW:
        case GL_DYNAMIC_DRAW:
            break;
        default:
            LOG(ERROR) << "keilingnica " << function << " invalid usage. ";
            set_gl_error(GL_INVALID_ENUM);
            return false;
    }
    return true;
} 

bool WebGLRenderingContext::ValidateCapability(const char* function, GLenum cap) {
    switch (cap) {
        case GL_BLEND:
        case GL_CULL_FACE:
        case GL_DEPTH_TEST:
        case GL_DITHER:
        case GL_POLYGON_OFFSET_FILL:
        case GL_SAMPLE_ALPHA_TO_COVERAGE:
        case GL_SAMPLE_COVERAGE:
        case GL_SCISSOR_TEST:
        case GL_STENCIL_TEST:
            return true;
        default:
            LOG(ERROR) << "keilingnica " << function << " invalid capability.";
            set_gl_error(GL_INVALID_ENUM);
            return false;
    }

}

void WebGLRenderingContext::DeleteBufferInMap(WebGLBuffer* buffer) {
    if (!buffer) return;
    buffer_map_.erase(buffer->webgl_id());
    delete buffer;
}

void WebGLRenderingContext::DeleteShaderInMap(WebGLShader* shader) {
    if (!shader) return;
    shader_map_.erase(shader->webgl_id());
    delete shader;
}

void WebGLRenderingContext::DeleteProgramInMap(WebGLShader* program) {
    if (!program) return;
    program_map_.erase(program->webgl_id());
    delete program;
}

void WebGLRenderingContext::DeleteTextureInMap(WebGLTexture* texture) {
    if (!texture) return;
    texture_map_.erase(texture->webgl_id());
    delete texture;
}

void WebGLRenderingContext::DeleteRenderbufferInMap(WebGLRenderbuffer* renderbuffer) {
    if (!renderbuffer) return;
    renderbuffer_map_.erase(renderbuffer->webgl_id());
    delete renderbuffer;
}

void WebGLRenderingContext::DeleteFramebufferInMap(WebGLFramebuffer* framebuffer) {
    if (!framebuffer) return;
    framebuffer_map_.erase(framebuffer->webgl_id());
    delete framebuffer;
}

bool WebGLRenderingContext::ValidateTextureBinding(const char* function, GLenum target, bool use_six_enums) {
  switch (target) {
    case GL_TEXTURE_2D:
      break;
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      if (use_six_enums)
        break;
    default:
      set_gl_error(GL_INVALID_ENUM);
      return false;
  }
  return true;
}

bool WebGLRenderingContext::ValidateStencilFunc(const char* function, GLenum func) {
    switch (func) {
        case GL_NEVER:
        case GL_LESS:
        case GL_LEQUAL:
        case GL_GREATER:
        case GL_GEQUAL:
        case GL_EQUAL:
        case GL_NOTEQUAL:
        case GL_ALWAYS:
            return true;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return false;
    }
}

bool WebGLRenderingContext::ValidateBlendFuncFactors(const char* function, GLenum src, GLenum dst) {
  if (((src == GL_CONSTANT_COLOR || src == GL_ONE_MINUS_CONSTANT_COLOR)
       && (dst == GL_CONSTANT_ALPHA || dst == GL_ONE_MINUS_CONSTANT_ALPHA))
      || ((dst == GL_CONSTANT_COLOR || dst == GL_ONE_MINUS_CONSTANT_COLOR)
          && (src == GL_CONSTANT_ALPHA || src == GL_ONE_MINUS_CONSTANT_ALPHA))) {
    set_gl_error(GL_INVALID_OPERATION);
    return false;
  }
  return true;
}

bool WebGLRenderingContext::ValidateBlendEquation(const char* function, GLenum mode) {
  switch (mode) {
    case GL_FUNC_ADD:
    case GL_FUNC_SUBTRACT:
    case GL_FUNC_REVERSE_SUBTRACT:
      return true;
    default:
      set_gl_error(GL_INVALID_ENUM);
      return false;
  }
}


bool WebGLRenderingContext::ValidateObject(WebGLObjectInterface* object) {
    if (object && !object->ValidateContext(this)) {
        set_gl_error(GL_INVALID_OPERATION);
        return false;
    }
    return true;
}

bool WebGLRenderingContext::RequireObject(const void* object) {
    if (!object) {
        set_gl_error(GL_INVALID_VALUE);
        return false;
    }
    return true;
}

void WebGLRenderingContext::set_gl_error(GLenum error) {
  if (gl_error_ == GL_NONE)
    gl_error_ = error;
}

GLenum WebGLRenderingContext::gl_error() {
  if (gl_error_ != GL_NONE) {
    GLenum err = gl_error_;
    gl_error_ = GL_NONE;
    return err;
  }
  return glGetError();
}

void WebGLRenderingContext::BufferData(
    GLenum target, nica::ArrayBufferView* buffer, GLenum usage) {
    if (!ValidateBufferDataParameters("bufferData", target, usage))
        return;
    glBufferData(target, buffer->ByteLength(), buffer->Data(), usage);
}

void WebGLRenderingContext::Enable(GLenum cap) {
    if (!ValidateCapability("enable", cap))
        return;
    glEnable(cap);
}

void WebGLRenderingContext::Viewport(
    GLint x, GLint y, GLsizei width, GLsizei height) {
    LOG(ERROR) << "keilingnica Viewport width " << width;
    LOG(ERROR) << "keilingnica Viewport height " << height;
    glViewport(x, y, width, height);
}

void WebGLRenderingContext::DrawElements(
    GLenum mode, GLsizei count, GLenum type, GLintptr offset) {
    glDrawElements(mode, count, type, reinterpret_cast<GLvoid*>(static_cast<intptr_t>(offset)));
    LOG(ERROR) << "keilingnica gl error " << glGetError() << " " << __func__;
    //todo(liqining): remove it, it should be triggered by vsync
}

void WebGLRenderingContext::DrawArrays(
    GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
    LOG(ERROR) << "keilingnica gl error " << glGetError() << " " << __func__;
}

void WebGLRenderingContext::Clear(GLbitfield mask) {
    if (mask & ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
        set_gl_error(GL_INVALID_VALUE);
        return;
    }
    glClear(mask);
}

void WebGLRenderingContext::ActiveTexture(GLenum texture) {
    glActiveTexture(texture);
}

void WebGLRenderingContext::BindTexture(GLenum target, WebGLTexture* texture) {
    switch (target) {
        case GL_TEXTURE_2D:
        case GL_TEXTURE_CUBE_MAP:
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return;
    }
    
    if (!ValidateObject(texture)) return;
    GLuint texture_id = texture ? texture->webgl_id() : 0;
    glBindTexture(target, texture_id);    
}

WebGLTexture* WebGLRenderingContext::CreateTexture() {
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    WebGLTexture* texture = new WebGLTexture(GetIsolate(), this, texture_id);
    texture_map_[texture_id] = texture;
    return texture;
}

void WebGLRenderingContext::DeleteTexture(WebGLTexture* texture) {
    if (!ValidateObject(texture)) 
        return;
    
    GLuint texture_id = texture ? texture->webgl_id() : 0;
    glDeleteTextures(1, &texture_id);
    DeleteTextureInMap(texture);
    return;
}

nica::ScriptValue WebGLRenderingContext::GetTexParameter(GLenum target, GLenum pname) {
    if (!ValidateTextureBinding("getTextParameter", target, false))
        return nica::ScriptValue::CreateNull(GetIsolate());
    switch (pname) {
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return nica::ScriptValue::CreateNull(GetIsolate());
    }
    GLint value = 0;
    glGetTexParameteriv(target, pname, &value);
    return WebGLAny(GetIsolate(), value);
}

GLboolean WebGLRenderingContext::IsTexture(WebGLTexture* texture) {
    if (!texture)
        return false;

    GLuint texture_id = texture ? texture->webgl_id() : 0;
    return glIsTexture(texture_id);
}

WebGLRenderbuffer* WebGLRenderingContext::CreateRenderbuffer() {
    GLuint renderbuffer_id = 0;
    glGenRenderbuffers(1, &renderbuffer_id);
    WebGLRenderbuffer* renderbuffer = new WebGLRenderbuffer(GetIsolate(), this, renderbuffer_id);
    renderbuffer_map_[renderbuffer_id] = renderbuffer;
    return renderbuffer;
}

void WebGLRenderingContext::BindRenderbuffer(GLenum target, WebGLRenderbuffer* renderbuffer) {
  if (target != GL_RENDERBUFFER) {
    set_gl_error(GL_INVALID_ENUM);
    return;
  }
  if (!ValidateObject(renderbuffer)) return;
  GLuint renderbuffer_id = renderbuffer ? renderbuffer->webgl_id() : 0;
  glBindRenderbuffer(target, renderbuffer_id);
}

void WebGLRenderingContext::DeleteRenderbuffer(WebGLRenderbuffer* renderbuffer) {
    if (!ValidateObject(renderbuffer)) 
        return;
    
    GLuint render_buffer_id = renderbuffer ? renderbuffer->webgl_id() : 0;
    glDeleteRenderbuffers(1, &render_buffer_id);
    DeleteRenderbufferInMap(renderbuffer);
}

nica::ScriptValue WebGLRenderingContext::GetRenderbufferParameter(GLenum target, GLenum pname) {
    if (target != GL_RENDERBUFFER) {
        set_gl_error(GL_INVALID_ENUM);
        return nica::ScriptValue::CreateNull(GetIsolate());
    }
    GLint value = 0;
    glGetRenderbufferParameteriv(target, pname, &value);
    switch (pname) {
        case GL_RENDERBUFFER_INTERNAL_FORMAT:
            return WebGLAny(GetIsolate(), static_cast<uint32_t>(value));
        case GL_RENDERBUFFER_WIDTH:
        case GL_RENDERBUFFER_HEIGHT:
        case GL_RENDERBUFFER_RED_SIZE:
        case GL_RENDERBUFFER_GREEN_SIZE:
        case GL_RENDERBUFFER_BLUE_SIZE:
        case GL_RENDERBUFFER_ALPHA_SIZE:
        case GL_RENDERBUFFER_DEPTH_SIZE:
        case GL_RENDERBUFFER_STENCIL_SIZE:
            return WebGLAny(GetIsolate(), value);
        default:
            set_gl_error(GL_INVALID_ENUM);
            return nica::ScriptValue::CreateNull(GetIsolate());
    }
}

bool WebGLRenderingContext::IsRenderbuffer(WebGLRenderbuffer* renderbuffer) {
  if (!renderbuffer)
    return false;
  GLuint renderbuffer_id = renderbuffer ? renderbuffer->webgl_id() : 0;
  return glIsRenderbuffer(renderbuffer_id);
}

void WebGLRenderingContext::RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    if (target != GL_RENDERBUFFER) {
        set_gl_error(GL_INVALID_ENUM);
        return;
    }
    switch (internalformat) {
        case GL_DEPTH_STENCIL:
            internalformat = GL_DEPTH24_STENCIL8;
            break;
        case GL_DEPTH_COMPONENT16:
            internalformat = GL_DEPTH_COMPONENT;
            break;
        case GL_RGBA4:
        case GL_RGB5_A1:
            internalformat = GL_RGBA;
            break;
        case GL_RGB565:
            internalformat = GL_RGB;
            break;
        case GL_STENCIL_INDEX8:
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return;
    }
    glRenderbufferStorage(target, internalformat, width, height);
}

void WebGLRenderingContext::BindFramebuffer(GLenum target, WebGLFramebuffer* framebuffer) {
    if (target != GL_FRAMEBUFFER) {
        set_gl_error(GL_INVALID_ENUM);
        return;
    }
    if (!ValidateObject(framebuffer)) return;

    GLuint framebuffer_id = framebuffer ? framebuffer->webgl_id() : 0;
    glBindFramebuffer(target, framebuffer_id);
}

GLenum WebGLRenderingContext::CheckFramebufferStatus(GLenum target) {
    if (target != GL_FRAMEBUFFER) {
        set_gl_error(GL_INVALID_ENUM);
        return static_cast<uint32_t>(0);
    }
    return glCheckFramebufferStatus(target);
}

WebGLFramebuffer* WebGLRenderingContext::CreateFramebuffer() {
    GLuint framebuffer_id = 0;
    glGenFramebuffers(1, &framebuffer_id);
    WebGLFramebuffer* framebuffer = new WebGLFramebuffer(GetIsolate(), this, framebuffer_id);
    framebuffer_map_[framebuffer_id] = framebuffer;
    return framebuffer;
}

void WebGLRenderingContext::DeleteFramebuffer(WebGLFramebuffer* framebuffer) {
    if (!ValidateObject(framebuffer)) return;
    GLuint framebuffer_id = framebuffer ? framebuffer->webgl_id() : 0;
    glDeleteFramebuffers(1, &framebuffer_id);
    DeleteFramebufferInMap(framebuffer);
}

// nica::ScriptValue GetFramebufferAttachmentParameter(GLenum target, GLenum attachment, GLenum pname) {

// }

void WebGLRenderingContext::BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    glBlendColor(red, green, blue, alpha);
}

void WebGLRenderingContext::BlendEquation(GLenum mode) {
    glBlendEquation(mode);
}

void WebGLRenderingContext::BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
    if (!ValidateBlendEquation("blendEquationSeparate", modeRGB))
        return;
    
    if (!ValidateBlendEquation("blendEquationSeparate", modeAlpha))
        return;
    glBlendEquationSeparate(modeRGB, modeAlpha);
}

void WebGLRenderingContext::BlendFunc(GLenum sfactor, GLenum dfactor) {
    if (!ValidateBlendFuncFactors("blendFunc", sfactor, dfactor))
        return;
    glBlendFunc(sfactor, dfactor);
    return;
}

void WebGLRenderingContext::BlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
    if (!ValidateBlendFuncFactors("blendFuncSeparate", srcRGB, dstRGB))
        return;
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void WebGLRenderingContext::StencilFunc(GLenum func, GLint ref, GLuint mask) {
    if (!ValidateStencilFunc("stencilFunc", func))
        return;
    glStencilFunc(func, ref, mask);
}

void WebGLRenderingContext::StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
    switch (face) {
        case GL_FRONT_AND_BACK:
        case GL_FRONT:
        case GL_BACK:
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return;
  }

  if (!ValidateStencilFunc("stencilFuncSeparate", func))
    return;

  glStencilFuncSeparate(face, func, ref, mask);
}

void WebGLRenderingContext::StencilMask(GLuint mask) {
    glStencilMask(mask);
}

void WebGLRenderingContext::StencilMaskSeparate(GLenum face, GLuint mask) {
    switch (face) {
        case GL_FRONT_AND_BACK:
        case GL_FRONT:
        case GL_BACK:
            break;
        default:
            set_gl_error(GL_INVALID_ENUM);
            return;
    }
    glStencilMaskSeparate(face, mask);
}

void WebGLRenderingContext::StencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
    glStencilOp(fail, zfail, zpass);
}

void WebGLRenderingContext::StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass) {
    glStencilOpSeparate(face, fail, zfail, zpass);
}

nica::FunctionTemplateBuilder 
WebGLRenderingContext::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    nica::FunctionTemplateBuilder builder(isolate, nullptr);
    builder.SetMethod("clear", &WebGLRenderingContext::Clear);
    builder.SetMethod("drawElements", &WebGLRenderingContext::DrawElements);
    builder.SetMethod("drawArrays", &WebGLRenderingContext::DrawArrays);
    builder.SetMethod("viewport", &WebGLRenderingContext::Viewport);
    builder.SetMethod("enable", &WebGLRenderingContext::Enable);
    builder.SetMethod("enableVertexAttribArray", &WebGLRenderingContext::EnableVertexAttribArray);
    builder.SetMethod("vertexAttribPointer", &WebGLRenderingContext::VertexAttribPointer);
    builder.SetMethod("getAttribLocation", &WebGLRenderingContext::GetAttribLocation);
    builder.SetMethod("compileShader", &WebGLRenderingContext::CompileShader);
    builder.SetMethod("shaderSource", &WebGLRenderingContext::ShaderSource);
    builder.SetMethod("createShader", &WebGLRenderingContext::CreateShader);
    builder.SetMethod("attachShader", &WebGLRenderingContext::AttachShader);
    builder.SetMethod("bufferData", &WebGLRenderingContext::BufferData);
    builder.SetMethod("useProgram", &WebGLRenderingContext::UseProgram);
    builder.SetMethod("createProgram", &WebGLRenderingContext::CreateProgram);
    builder.SetMethod("linkProgram", &WebGLRenderingContext::LinkProgram);
    builder.SetMethod("createBuffer", &WebGLRenderingContext::CreateBuffer);    
    builder.SetMethod("bindBuffer", &WebGLRenderingContext::BindBuffer);
    builder.SetMethod("clearColor", &WebGLRenderingContext::ClearColor);
    builder.SetMethod("activeTexture", &WebGLRenderingContext::ActiveTexture);
    builder.SetMethod("bindTexture", &WebGLRenderingContext::BindTexture);
    builder.SetMethod("createTexture", &WebGLRenderingContext::CreateTexture);
    builder.SetMethod("GetTexParameter", &WebGLRenderingContext::GetTexParameter);
    builder.SetMethod("deleteTexture", &WebGLRenderingContext::DeleteTexture);
    builder.SetMethod("isTexture", &WebGLRenderingContext::IsTexture);
    builder.SetMethod("bindRenderbuffer", &WebGLRenderingContext::BindRenderbuffer);
    builder.SetMethod("deleteRenderbuffer", &WebGLRenderingContext::DeleteRenderbuffer);
    builder.SetMethod("getRenderbufferParameter", &WebGLRenderingContext::GetRenderbufferParameter);
    builder.SetMethod("isRenderbuffer", &WebGLRenderingContext::IsRenderbuffer);
    builder.SetMethod("bindFramebuffer", &WebGLRenderingContext::BindFramebuffer);
    builder.SetMethod("checkFramebufferStatus", &WebGLRenderingContext::CheckFramebufferStatus);
    builder.SetMethod("createFramebuffer", &WebGLRenderingContext::CreateFramebuffer);
    builder.SetMethod("deleteFramebuffer", &WebGLRenderingContext::DeleteFramebuffer);
    builder.SetMethod("blendColor", &WebGLRenderingContext::BlendColor);
    builder.SetMethod("blendEquation", &WebGLRenderingContext::BlendEquation);
    builder.SetMethod("blendEquationSeparate", &WebGLRenderingContext::BlendEquationSeparate);
    builder.SetMethod("blendFunc", &WebGLRenderingContext::BlendFunc);
    builder.SetMethod("blendFuncSeparate", &WebGLRenderingContext::BlendFuncSeparate);
    builder.SetMethod("stencilFunc", &WebGLRenderingContext::StencilFunc);
    builder.SetMethod("stencilFuncSeparate", &WebGLRenderingContext::StencilFuncSeparate);
    builder.SetMethod("stencilMask", &WebGLRenderingContext::StencilMask);
    builder.SetMethod("stencilMaskSeparate", &WebGLRenderingContext::StencilMaskSeparate);
    builder.SetMethod("stencilOp", &WebGLRenderingContext::StencilOp);
    builder.SetMethod("stencilOpSeparate", &WebGLRenderingContext::StencilOpSeparate);

#define WEBGL_CONSTANT(name, val) builder.SetValue(#name, val)
#include "binding/modules/webgl/webgl_context_const_value.h"    
#undef WEBGL_CONSTANT
    return builder;
}

} // namespace bind
