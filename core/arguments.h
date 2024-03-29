#ifndef CORE_ARGUMENTS_H_
#define CORE_ARGUMENTS_H_

#include "core/converter.h"
#include "v8-template.h"
#include "v8-forward.h"
#include "v8-external.h"
#include "base/logging.h"
namespace nica {

class Arguments {
 public:
  Arguments();
  explicit Arguments(const v8::FunctionCallbackInfo<v8::Value>& info);
  explicit Arguments(const v8::PropertyCallbackInfo<v8::Value>& info);
  ~Arguments();

  template <typename T>
  bool GetHolder(T* out) const {
    v8::Local<v8::Object> holder = is_for_property_
                                       ? info_for_property_->Holder()
                                       : info_for_function_->Holder();
    if (holder.IsEmpty())
      LOG(ERROR) << "holder empty";
    return ConvertFromV8(isolate_, holder, out);
  }

  template<typename T>
  bool GetData(T* out) {
    v8::Local<v8::Value> data = is_for_property_ ? info_for_property_->Data()
                                                 : info_for_function_->Data();
    return ConvertFromV8(isolate_, data, out);
  }

  template<typename T>
  bool GetNext(T* out) {
    if (is_for_property_ || next_ >= info_for_function_->Length()) {
      insufficient_arguments_ = true;
      return false;
    }
    v8::Local<v8::Value> val = (*info_for_function_)[next_++];
    return ConvertFromV8(isolate_, val, out);
  }

  template<typename T>
  bool GetRemaining(std::vector<T>* out) {
    if (is_for_property_ || next_ >= info_for_function_->Length()) {
      insufficient_arguments_ = true;
      return false;
    }
    int remaining = info_for_function_->Length() - next_;
    out->resize(remaining);
    for (int i = 0; i < remaining; ++i) {
      v8::Local<v8::Value> val = (*info_for_function_)[next_++];
      if (!ConvertFromV8(isolate_, val, &out->at(i)))
        return false;
    }
    return true;
  }

  bool Skip() {
    if (is_for_property_)
      return false;
    if (next_ >= info_for_function_->Length())
      return false;
    next_++;
    return true;
  }

  int Length() const {
    return is_for_property_ ? 0 : info_for_function_->Length();
  }

  template<typename T>
  void Return(T val) {
    v8::Local<v8::Value> v8_value;
    if (!TryConvertToV8(isolate_, val, &v8_value))
      return;
    (is_for_property_ ? info_for_property_->GetReturnValue()
                      : info_for_function_->GetReturnValue())
        .Set(v8_value);
  }

  v8::Local<v8::Value> PeekNext() const;

  std::vector<v8::Local<v8::Value>> GetAll() const;

  void ThrowError() const;
  void ThrowTypeError(const std::string& message) const;

  v8::Isolate* isolate() const { return isolate_; }
  const v8::FunctionCallbackInfo<v8::Value>* function_callback_info() { return info_for_function_; }
  const v8::PropertyCallbackInfo<v8::Value>* property_callback_info() { return info_for_property_; }

  bool IsConstructCall() const;

 private:
  v8::Isolate* isolate_;
  union {
    const v8::FunctionCallbackInfo<v8::Value>* info_for_function_;
    const v8::PropertyCallbackInfo<v8::Value>* info_for_property_;
  };
  int next_ = 0;
  bool insufficient_arguments_ = false;
  bool is_for_property_ = false;
};

}  // namespace nica

#endif  // NICA_ARGUMENTS_H_
