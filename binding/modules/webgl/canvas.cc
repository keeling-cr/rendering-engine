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


void GetCanvasWidth(v8::Local<v8::String> property,
               const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> self = info.Holder();
  v8::Local<v8::External> wrap =
      v8::Local<v8::External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<Canvas*>(ptr)->width();
  info.GetReturnValue().Set(value);
}

void SetCanvasWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value,
               const v8::PropertyCallbackInfo<void>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::Local<v8::Object> self = info.Holder();
  v8::Local<v8::External> wrap =
      v8::Local<v8::External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int x; 
  if (nica::ConvertFromV8(isolate, value, &x))
    static_cast<Canvas*>(ptr)->SetWidth(x);
}

void GetCanvasHeight(v8::Local<v8::String> property,
               const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> self = info.Holder();
  v8::Local<v8::External> wrap =
      v8::Local<v8::External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<Canvas*>(ptr)->height();
  info.GetReturnValue().Set(value);
}

void SetCanvasHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value,
               const v8::PropertyCallbackInfo<void>& info) {
  v8::Isolate* isolate = info.GetIsolate();
  v8::Local<v8::Object> self = info.Holder();
  v8::Local<v8::External> wrap =
      v8::Local<v8::External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int x; 
  if (nica::ConvertFromV8(isolate, value, &x))
    static_cast<Canvas*>(ptr)->SetHeight(x);
}

} // namespace

Canvas::Canvas(int width, int height): width_(width), height_(height) {}

Canvas::~Canvas() = default;

void Canvas::Register(nica::JSIsolate* js_isolate, nica::JSContext* js_context) {
    v8::Isolate* isolate = js_isolate->isolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = js_context->context();
    v8::Context::Scope context_scope(context);

    v8::Local<v8::FunctionTemplate> canvas_constructor_temp = 
        v8::FunctionTemplate::New(isolate, CanvasNew);
    
    canvas_constructor_temp->SetClassName(
      nica::StringToSymbol(isolate, "Canvas"));
    canvas_constructor_temp->ReadOnlyPrototype();
    
    canvas_constructor_temp->InstanceTemplate()->SetInternalFieldCount(1);
    canvas_constructor_temp->InstanceTemplate()->SetAccessor(
        nica::StringToSymbol(isolate, "width"), GetCanvasWidth, SetCanvasWidth);
    canvas_constructor_temp->InstanceTemplate()->SetAccessor(
        nica::StringToSymbol(isolate, "height"), GetCanvasHeight, SetCanvasHeight);

    v8::Local<v8::Function> canvas_constructor_func = 
        canvas_constructor_temp->GetFunction(context).ToLocalChecked();

    context->Global()
        ->Set(context, nica::StringToV8(isolate, "canvas"), canvas_constructor_func)
        .FromJust();
  }

} // namespace bind
