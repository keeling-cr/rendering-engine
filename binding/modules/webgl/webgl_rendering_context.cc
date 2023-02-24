
#include "angle/shader_util.h"
#include "angle/debug_util.h"
#include "binding/modules/webgl/webgl_rendering_context.h"
#include "binding/modules/webgl/webgl_buffer.h"
#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_shader.h"
#include "binding/modules/webgl/webgl_program.h"
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
    glViewport(x, y, width, height);
}

void WebGLRenderingContext::DrawElements(
    GLenum mode, GLsizei count, GLenum type, GLintptr offset) {
    glDrawElements(mode, count, type, reinterpret_cast<GLvoid*>(static_cast<intptr_t>(offset)));
}

void WebGLRenderingContext::Clear(GLbitfield mask) {
    if (mask & ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
        set_gl_error(GL_INVALID_VALUE);
        return;
    }
    glClear(mask);
}

nica::FunctionTemplateBuilder 
WebGLRenderingContext::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    nica::FunctionTemplateBuilder builder(isolate, nullptr);
    builder.SetMethod("clear", std::function<void(WebGLRenderingContext*, GLbitfield)>{&WebGLRenderingContext::Clear});
    builder.SetMethod("drawElements", std::function<void(WebGLRenderingContext*, GLenum, GLsizei, GLenum, GLintptr)>{&WebGLRenderingContext::DrawElements});
    builder.SetMethod("viewport", std::function<void(WebGLRenderingContext*, GLint, GLint, GLsizei, GLsizei)>{&WebGLRenderingContext::Viewport});
    builder.SetMethod("enable", std::function<void(WebGLRenderingContext*, GLenum)>{&WebGLRenderingContext::Enable});
    builder.SetMethod("enableVertexAttribArray", std::function<void(WebGLRenderingContext*, GLuint)>{&WebGLRenderingContext::EnableVertexAttribArray});
    builder.SetMethod("vertexAttribPointer", std::function<void(WebGLRenderingContext*,GLuint, GLint, GLenum, GLboolean, GLsizei, GLintptr)>{&WebGLRenderingContext::VertexAttribPointer});
    builder.SetMethod("getAttribLocation", std::function<GLint(WebGLRenderingContext*, WebGLProgram*, const std::string&)>{&WebGLRenderingContext::GetAttribLocation});
    builder.SetMethod("compileShader", std::function<void(WebGLRenderingContext*, WebGLShader*)>{&WebGLRenderingContext::CompileShader});
    builder.SetMethod("shaderSource", std::function<void(WebGLRenderingContext*, WebGLShader*, const std::string&)>{&WebGLRenderingContext::ShaderSource});
    builder.SetMethod("createShader", std::function<WebGLShader*(WebGLRenderingContext*, GLenum)>{&WebGLRenderingContext::CreateShader});
    builder.SetMethod("attachShader", std::function<void(WebGLRenderingContext*, WebGLProgram*, WebGLShader*)>{&WebGLRenderingContext::AttachShader});
    builder.SetMethod("bufferData", std::function<void(WebGLRenderingContext*, GLenum, nica::ArrayBufferView*, GLenum)>{&WebGLRenderingContext::BufferData});
    builder.SetMethod("useProgram", std::function<void(WebGLRenderingContext*, WebGLProgram*)>{&WebGLRenderingContext::UseProgram});
    builder.SetMethod("createProgram", std::function<WebGLProgram*(WebGLRenderingContext*)>{&WebGLRenderingContext::CreateProgram});
    builder.SetMethod("linkProgram", std::function<void(WebGLRenderingContext*, WebGLProgram*)>{&WebGLRenderingContext::LinkProgram});
    builder.SetMethod("createBuffer", std::function<WebGLBuffer*(WebGLRenderingContext*)>{&WebGLRenderingContext::CreateBuffer});    
    builder.SetMethod("bindBuffer", std::function<void(WebGLRenderingContext*, GLenum, WebGLBuffer*)>{&WebGLRenderingContext::BindBuffer});
    builder.SetMethod("clearColor", std::function<void(WebGLRenderingContext*, float, float, float, float)>{&WebGLRenderingContext::ClearColor});

#define WEBGL_CONSTANT(name, val) builder.SetValue(#name, val)
#include "binding/modules/webgl/webgl_context_const_value.h"    
#undef WEBGL_CONSTANT
    return builder;
}

} // namespace bind
