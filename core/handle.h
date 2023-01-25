#ifndef CORE_HANDLE_H_
#define CORE_HANDLE_H_

#include "core/converter.h"

namespace nica {

template<typename T>
class Handle {
 public:
  Handle() : object_(nullptr) {}

  Handle(v8::Local<v8::Value> wrapper, T* object)
    : wrapper_(wrapper),
      object_(object) {
  }

  bool IsEmpty() const { return !object_; }

  void Clear() {
    wrapper_.Clear();
    object_ = NULL;
  }

  T* operator->() const { return object_; }
  v8::Local<v8::Value> ToV8() const { return wrapper_; }
  T* get() const { return object_; }

 private:
  v8::Local<v8::Value> wrapper_;
  T* object_;
};

template<typename T>
struct Converter<nica::Handle<T> > {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    const nica::Handle<T>& val) {
    return val.ToV8();
  }
  static bool FromV8(v8::Isolate* isolate, v8::Local<v8::Value> val,
                     nica::Handle<T>* out) {
    T* object = NULL;
    if (!Converter<T*>::FromV8(isolate, val, &object)) {
      return false;
    }
    *out = nica::Handle<T>(val, object);
    return true;
  }
};

template<typename T>
nica::Handle<T> CreateHandle(v8::Isolate* isolate, T* object) {
  v8::Local<v8::Object> wrapper;
  if (!object->GetWrapper(isolate).ToLocal(&wrapper) || wrapper.IsEmpty())
    return nica::Handle<T>();
  return nica::Handle<T>(wrapper, object);
}

}  // namespace nica 

#endif  // CORE_HANDLE_H_
