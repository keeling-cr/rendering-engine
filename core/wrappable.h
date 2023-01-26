#ifndef CORE_WRAPPABLE_H_
#define CORE_WRAPPABLE_H_

#include <type_traits>

#include "core/converter.h"
#include "core/wrapper_info.h"

namespace nica {

namespace internal {

void* FromV8Impl(v8::Isolate* isolate,
                            v8::Local<v8::Value> val,
                            WrapperInfo* info);

}  // namespace internal

class ObjectTemplateBuilder;

class WrappableBase {
 public:
  WrappableBase(const WrappableBase&) = delete;
  WrappableBase& operator=(const WrappableBase&) = delete;

 protected:
  WrappableBase();
  virtual ~WrappableBase();

  virtual ObjectTemplateBuilder GetObjectTemplateBuilder(v8::Isolate* isolate);

  virtual const char* GetTypeName();

  v8::MaybeLocal<v8::Object> GetWrapperImpl(v8::Isolate* isolate,
                                            WrapperInfo* wrapper_info);

 private:
  static void FirstWeakCallback(
      const v8::WeakCallbackInfo<WrappableBase>& data);
  static void SecondWeakCallback(
      const v8::WeakCallbackInfo<WrappableBase>& data);

  bool dead_ = false;
  v8::Global<v8::Object> wrapper_;  // Weak
};


template<typename T>
class Wrappable : public WrappableBase {
 public:
  Wrappable(const Wrappable&) = delete;
  Wrappable& operator=(const Wrappable&) = delete;

  v8::MaybeLocal<v8::Object> GetWrapper(v8::Isolate* isolate) {
    return GetWrapperImpl(isolate, &T::kWrapperInfo);
  }

 protected:
  Wrappable() = default;
  ~Wrappable() override = default;
};

template <typename T>
struct ToV8ReturnsMaybe<
    T*,
    typename std::enable_if<
        std::is_convertible<T*, WrappableBase*>::value>::type> {
  static const bool value = true;
};

template <typename T>
struct Converter<T*,
                 typename std::enable_if<
                     std::is_convertible<T*, WrappableBase*>::value>::type> {
  static v8::MaybeLocal<v8::Value> ToV8(v8::Isolate* isolate, T* val) {
    if (val == nullptr)
      return v8::Null(isolate);
    v8::Local<v8::Object> wrapper;
    if (!val->GetWrapper(isolate).ToLocal(&wrapper))
      return v8::MaybeLocal<v8::Value>();
    return v8::MaybeLocal<v8::Value>(wrapper);
  }

  static bool FromV8(v8::Isolate* isolate, v8::Local<v8::Value> val, T** out) {
    *out = static_cast<T*>(static_cast<WrappableBase*>(
        internal::FromV8Impl(isolate, val, &T::kWrapperInfo)));
    return *out != NULL;
  }
};

}  // namespace nica 

#endif  // CORE_WRAPPABLE_H_
