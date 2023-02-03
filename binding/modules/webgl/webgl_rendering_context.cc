#include "binding/modules/webgl/webgl_rendering_context.h"
#include "binding/modules/webgl/webgl_buffer.h"
#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_shader.h"
#include "binding/modules/webgl/webgl_program.h"
#include "base/logging.h"

namespace bind {

namespace {

// settings
const unsigned int SCR_WIDTH = 300;
const unsigned int SCR_HEIGHT = 300;

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

} // namespace

unsigned long WebGLRenderingContext::s_context_counter = 0;

nica::WrapperInfo WebGLRenderingContext::kWrapperInfo = {
    nica::kEmbedderNicaMain};

WebGLRenderingContext:: WebGLRenderingContext(v8::Isolate* isolate)
    : V8Object(isolate)
    , context_id_(s_context_counter++) {
    InitGlfw();
}

WebGLRenderingContext::~WebGLRenderingContext() {
    DeleteMapObjects(buffer_map_);
    DeleteMapObjects(shader_map_);
    DeleteMapObjects(program_map_);
}


bool WebGLRenderingContext::InitGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif
    window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "opengl_190715", NULL, NULL);
    if (window_ == nullptr) {
        LOG(ERROR) << "Failed to create GLFW window";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG(ERROR) << "Failed to initialize GLAD";
        return false;
    }
    return true;
}

void WebGLRenderingContext::ClearColor(
    float read, float green, float blue, float alpha) {
    glClearColor(read, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window_);

    while(!glfwWindowShouldClose(window_)) {
        processInput(window_);
        glfwPollEvents(); // 检查触发事件
    }
}

WebGLBuffer* WebGLRenderingContext::CreateBuffer() {
    GLuint buffer_id = 0;
    glGenBuffers(1, &buffer_id);
    WebGLBuffer* buffer = new WebGLBuffer(GetIsolate(), this, buffer_id);
    buffer_map_[buffer_id] = buffer;
    return buffer;
}

WebGLShader* WebGLRenderingContext::CreateShader(GLenum type) {
    LOG(ERROR) << "keilingnica webgl " << __func__;
    GLuint shader_id = glCreateShader(type);
    WebGLShader* shader = new WebGLShader(GetIsolate(), this, shader_id);
    shader_map_[shader_id] = shader;
    return shader; 
}

void WebGLRenderingContext::ShaderSource(WebGLShader* shader, const std::string& source) {
    LOG(ERROR) << "keilingnica webgl " << __func__;
    if (!RequireObject(shader)) 
        return;
    if (!ValidateObject(shader))
        return;
    shader->set_source(source);
}

void WebGLRenderingContext::CompileShader(WebGLShader* shader) {
    LOG(ERROR) << "keilingnica webgl " << __func__;
    if (!RequireObject(shader)) 
        return;
    if (!ValidateObject(shader)) 
        return;
    
    GLuint shader_id = shader->webgl_id();
    GLint shader_type = 0;
    glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);
    if (shader_type == 0)
        return;
    
    const char* shader_code = shader->source();

    glShaderSource(shader_id, 1, &shader_code, NULL);
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
    LOG(ERROR) << "keilingnica " << __func__;
    if (!RequireObject(program)) 
        return -1;
    if (!ValidateObject(program))
        return -1;

    GLuint program_id = program->webgl_id();
    GLint location = glGetAttribLocation(program_id, name.c_str());
    return location;
}

void WebGLRenderingContext::BindBuffer(GLenum target, WebGLBuffer* buffer) {
    LOG(ERROR) << "keilingnica " << __func__;
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

nica::FunctionTemplateBuilder 
WebGLRenderingContext::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    nica::FunctionTemplateBuilder builder(isolate, nullptr);
    builder.SetMethod("getAttribLocation", std::function<GLint(WebGLRenderingContext*, WebGLProgram*, const std::string&)>{&WebGLRenderingContext::GetAttribLocation});
    builder.SetMethod("attachShader", std::function<void(WebGLRenderingContext*, WebGLProgram*, WebGLShader*)>{&WebGLRenderingContext::AttachShader});
    builder.SetMethod("useProgram", std::function<void(WebGLRenderingContext*, WebGLProgram*)>{&WebGLRenderingContext::UseProgram});
    builder.SetMethod("createProgram", std::function<void(WebGLRenderingContext*)>{&WebGLRenderingContext::CreateProgram});
    builder.SetMethod("linkProgram", std::function<void(WebGLRenderingContext*, WebGLProgram*)>{&WebGLRenderingContext::LinkProgram});
    builder.SetMethod("shaderSource", std::function<void(WebGLRenderingContext*, WebGLShader*, const std::string&)>{&WebGLRenderingContext::ShaderSource});
    builder.SetMethod("createShader", std::function<WebGLShader*(WebGLRenderingContext*, GLenum)>{&WebGLRenderingContext::CreateShader});
    builder.SetMethod("createBuffer", std::function<WebGLBuffer*(WebGLRenderingContext*)>{&WebGLRenderingContext::CreateBuffer});    
    builder.SetMethod("bindBuffer", std::function<void(WebGLRenderingContext*, GLenum, WebGLBuffer*)>{&WebGLRenderingContext::BindBuffer});
    builder.SetMethod("clearColor", std::function<void(WebGLRenderingContext*, float, float, float, float)>{&WebGLRenderingContext::ClearColor});

#define WEBGL_CONSTANT(name, val) builder.SetValue(#name, val)
#include "binding/modules/webgl/webgl_context_const_value.h"    
#undef WEBGL_CONSTANT
    return builder;
}

} // namespace bind
