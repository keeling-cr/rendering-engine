#include "core/v8_object.h"


namespace nica {

V8ObjectBase::~V8ObjectBase() = default;

void V8ObjectBase::SetInstance(
    v8::Isolate* isolate, 
    v8::Handle<v8::Object> instance, 
    WrapperInfo* info,
    bool weak) {
    instance_ = v8::Local<v8::Object>::New(isolate, instance);

    int indices[] = {kWrapperInfoIndex, kEncodedValueIndex};
    void* values[] = {info, this};
    instance_->SetAlignedPointerInInternalFields(2, indices, values);
// todo(liqining): v8 gc
//   if (weak) {
//     instance_.MarkIndependent();
//     instance_.MakeWeak(this, WeakCallback);
//   }
}

void V8ObjectBase::WeakCallback(v8::Persistent<v8::Value> value, void* data) {
  V8ObjectBase* object = static_cast<V8ObjectBase*>(data);
  delete object;
}
} // namespace nica