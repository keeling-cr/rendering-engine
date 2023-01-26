#include "core/wrappable.h"

#include "base/check_op.h"
#include "core/object_template_builder.h"
#include "core/per_isolate_data.h"

namespace nica {

WrappableBase::WrappableBase() = default;

WrappableBase::~WrappableBase() {
  wrapper_.Reset();
}

ObjectTemplateBuilder WrappableBase::GetObjectTemplateBuilder(
    v8::Isolate* isolate) {
  return ObjectTemplateBuilder(isolate, GetTypeName());
}

const char* WrappableBase::GetTypeName() {
  return nullptr;
}

void WrappableBase::FirstWeakCallback(
    const v8::WeakCallbackInfo<WrappableBase>& data) {
  WrappableBase* wrappable = data.GetParameter();
  wrappable->dead_ = true;
  wrappable->wrapper_.Reset();
  data.SetSecondPassCallback(SecondWeakCallback);
}

void WrappableBase::SecondWeakCallback(
    const v8::WeakCallbackInfo<WrappableBase>& data) {
  WrappableBase* wrappable = data.GetParameter();
  delete wrappable;
}

v8::MaybeLocal<v8::Object> WrappableBase::GetWrapperImpl(v8::Isolate* isolate,
                                                         WrapperInfo* info) {
  if (!wrapper_.IsEmpty()) {
    return v8::MaybeLocal<v8::Object>(
        v8::Local<v8::Object>::New(isolate, wrapper_));
  }

  if (dead_)
    return v8::MaybeLocal<v8::Object>();

  PerIsolateData* data = PerIsolateData::From(isolate);
  v8::Local<v8::ObjectTemplate> templ = data->GetObjectTemplate(info);
  if (templ.IsEmpty()) {
    templ = GetObjectTemplateBuilder(isolate).Build();
    CHECK(!templ.IsEmpty());
    data->SetObjectTemplate(info, templ);
  }
  CHECK_EQ(kNumberOfInternalFields, templ->InternalFieldCount());
  v8::Local<v8::Object> wrapper;
  if (!templ->NewInstance(isolate->GetCurrentContext()).ToLocal(&wrapper)) {
    delete this;
    return v8::MaybeLocal<v8::Object>(wrapper);
  }

  int indices[] = {kWrapperInfoIndex, kEncodedValueIndex};
  void* values[] = {info, this};
  wrapper->SetAlignedPointerInInternalFields(2, indices, values);
  wrapper_.Reset(isolate, wrapper);
  wrapper_.SetWeak(this, FirstWeakCallback, v8::WeakCallbackType::kParameter);
  return v8::MaybeLocal<v8::Object>(wrapper);
}

namespace internal {

void* FromV8Impl(v8::Isolate* isolate, v8::Local<v8::Value> val,
                 WrapperInfo* wrapper_info) {
  if (!val->IsObject())
    return NULL;
  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(val);
  WrapperInfo* info = WrapperInfo::From(obj);

  if (!info)
    return NULL;

  if (info != wrapper_info)
    return NULL;

  return obj->GetAlignedPointerFromInternalField(kEncodedValueIndex);
}

}  // namespace internal

}  // namespace nica 
