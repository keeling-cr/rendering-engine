#include "binding/modules/webgl/webgl_rendering_context.h"
#include "binding/modules/webgl/webgl_buffer.h"
#include "binding/modules/webgl/webgl_object.h"
#include "binding/modules/webgl/webgl_shader.h"
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
    bool ok = true;
    GLuint shader_id = glCreateShader(type);
    WebGLShader* shader = new WebGLShader(GetIsolate(), this, shader_id);
    shader_map_[shader_id] = shader;
    return shader; 
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

bool WebGLRenderingContext::ValidateObject(WebGLObjectInterface* object) {
  if (object && !object->ValidateContext(this)) {
    set_gl_error(GL_INVALID_OPERATION);
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
