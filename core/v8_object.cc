#include "core/v8_object.h"


namespace nica {

V8ObjectBase::~V8ObjectBase() = default;

void V8ObjectBase::SetInstance(
    v8::Isolate* isolate, 
    v8::Handle<v8::Object> instance, 
    WrapperInfo* info,
    bool weak) {
    v8::Local<v8::Object> instance_local = v8::Local<v8::Object>::New(isolate, instance);
   
    int indices[] = {kWrapperInfoIndex, kEncodedValueIndex};
    void* values[] = {info, this};
    instance_local->SetAlignedPointerInInternalFields(2, indices, values);

    instance_.Reset(isolate, instance_local);
    instance_.SetWeak(this, &V8ObjectBase::FirstWeakCallback,
        v8::WeakCallbackType::kParameter);
}

// static
void V8ObjectBase::FirstWeakCallback(
    const v8::WeakCallbackInfo<V8ObjectBase>& data) {
  data.GetParameter()->instance_.Reset();
  data.SetSecondPassCallback(SecondWeakCallback);
}

// static
void V8ObjectBase::SecondWeakCallback(
    const v8::WeakCallbackInfo<V8ObjectBase>& data) {
  delete data.GetParameter();
}

} // namespace nica