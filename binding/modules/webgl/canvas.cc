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
    Canvas * c = new Canvas(x, y);

    args.Holder()->SetInternalField(0, v8::External::New(isolate, c));

    // // return the new object back to the javascript caller
    args.GetReturnValue().Set(args.Holder());
}

} // namespace

nica::WrapperInfo Canvas::kWrapperInfo = {
    nica::kEmbedderNicaMain};

Canvas::Canvas(int width, int height): width_(width), height_(height) {}

Canvas::~Canvas() = default;


nica::FunctionTemplateBuilder 
Canvas::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    return nica::FunctionTemplateBuilder(isolate, CanvasNew)
            .SetProperty("width", std::function<int(Canvas*)>{&Canvas::width}, std::function<void(Canvas*, int)>{&Canvas::SetWidth})
            .SetProperty("height", std::function<int(Canvas*)>{&Canvas::height}, std::function<void(Canvas*, int)>{&Canvas::SetHeight});
}

void Canvas::Register(
        nica::JSIsolate* js_isolate, nica::JSContext* js_context) {
    v8::Isolate* isolate = js_isolate->isolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = js_context->context();
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Object> global = context->Global();
    global
        ->Set(context, nica::StringToV8(isolate, "canvas"),
            GetFunctionTemplateBuilder(isolate).
            Build()->GetFunction(context).ToLocalChecked())
            .ToChecked();    
}

} // namespace bind
