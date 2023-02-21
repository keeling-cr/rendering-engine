#include "binding/modules/webgl/canvas.h"

#include "angle/config_params.h"
#include "angle/angle_util.h"
#include "base/logging.h"

namespace bind {

namespace {
const char* kAngleWindowName = "nica";
const int kAngleRedBits = 8;
const int kAngleGreenBits = 8;
const int kAngleBlueBits = 8;
const int kAngleAlphaBits = 8;
const int kAngleDepthBits = 24;
const int kAngleStencilBits = 8;

void CanvasNew(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);

    if (!args.IsConstructCall()) {
        isolate->ThrowError("Canvas must be constructed with new");
        return;
    }

    int x, y;
    if (!nica::ConvertFromV8(isolate, args[0], &x) ||
            !nica::ConvertFromV8(isolate, args[1], &y)) {
        isolate->ThrowError("Invalid arguments");
        return; 
    }
    // todo(liqining): memory leak
    Canvas * c = new Canvas(isolate, args.Holder(), x, y);

    // // return the new object back to the javascript caller
    args.GetReturnValue().Set(args.Holder());
}

} // namespace

nica::WrapperInfo Canvas::kWrapperInfo = {
    nica::kEmbedderNicaMain};

Canvas::Canvas(
    v8::Isolate* isolate, v8::Local<v8::Object> instance,
    int width, int height)
    : V8Object(isolate, instance)
    , width_(width)
    , height_(height) {
    // EGL2.0
    EGLenum egl_client_type = EGL_OPENGL_ES_API;
    EGLint gl_major_version = 2;
    EGLint gl_minor_version = 0;
    EGLint profile_mask    = 0;

    os_window_.reset(angle::OSWindow::New());
    egl_window_.reset(angle::EGLWindow::New(
        egl_client_type, gl_major_version, gl_minor_version, profile_mask));
    os_window_->SetVisible(true);
    
    if (!os_window_->Initialize(kAngleWindowName, width, height)){
        LOG(ERROR) << "keilingnica initialize os window error";
    }

    angle::ConfigParameters config_params;
    config_params.red_bits     = kAngleRedBits;
    config_params.green_bits   = kAngleGreenBits;
    config_params.blue_bits    = kAngleBlueBits;
    config_params.alpha_bits   = kAngleAlphaBits;
    config_params.depth_bits   = kAngleDepthBits;
    config_params.stencil_bits = kAngleStencilBits;

    if (!egl_window_->InitializeGL(os_window_.get(), angle::AngleEGLLibrary(), config_params)) {
        LOG(ERROR) << "keilingnica initialize gl error";
    }

    // Disable vsync
    if (!egl_window_->SetSwapInterval(0)) {
        LOG(ERROR) << "keilingnica swap interval erro";
    }
}

Canvas::~Canvas() = default;


WebGLRenderingContext* Canvas::GetContext() {
    if (!webgl_rendering_context_) {
        webgl_rendering_context_ = 
            std::unique_ptr<WebGLRenderingContext>(new WebGLRenderingContext(GetIsolate()));
    }
    LOG(ERROR) << "keilingnica " << __func__;
    return webgl_rendering_context_.get();
}

nica::FunctionTemplateBuilder 
Canvas::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    return nica::FunctionTemplateBuilder(isolate, CanvasNew)
            .SetMethod("getContext", std::function<WebGLRenderingContext*(Canvas*)>{&Canvas::GetContext})
            .SetProperty("width", std::function<int(Canvas*)>{&Canvas::width}, std::function<void(Canvas*, int)>{&Canvas::SetWidth})
            .SetProperty("height", std::function<int(Canvas*)>{&Canvas::height}, std::function<void(Canvas*, int)>{&Canvas::SetHeight});
}

} // namespace bind
