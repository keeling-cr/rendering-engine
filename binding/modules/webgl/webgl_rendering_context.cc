#include "binding/modules/webgl/webgl_rendering_context.h"
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

WebGLRenderingContext::~WebGLRenderingContext() = default;


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

nica::FunctionTemplateBuilder 
WebGLRenderingContext::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    return nica::FunctionTemplateBuilder(isolate, nullptr)
        .SetMethod("clearColor", std::function<void(WebGLRenderingContext*, float, float, float, float)>{&WebGLRenderingContext::ClearColor});
}

} // namespace bind
