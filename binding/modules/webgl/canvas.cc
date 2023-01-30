#include "binding/modules/webgl/canvas.h"
#include "base/logging.h"

namespace bind {

namespace {
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
    , height_(height) {}

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
