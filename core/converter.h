#ifndef CORE_CONVERTER_H_
#define CORE_CONVERTER_H_

#include <stdint.h>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "v8-container.h"
#include "v8-forward.h"
#include "v8-isolate.h"

namespace nica {

template <typename T, typename Enable = void>
struct ToV8ReturnsMaybe {
  static const bool value = false;
};

template<typename T, typename Enable = void>
struct Converter {};

template<>
struct Converter<bool> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    bool val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     bool* out);
};

template<>
struct Converter<int32_t> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    int32_t val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     int32_t* out);
};

template<>
struct Converter<uint32_t> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    uint32_t val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     uint32_t* out);
};

template<>
struct Converter<int64_t> {
  // Warning: JavaScript cannot represent 64 integers precisely.
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    int64_t val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     int64_t* out);
};

template<>
struct Converter<uint64_t> {
  // Warning: JavaScript cannot represent 64 integers precisely.
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    uint64_t val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     uint64_t* out);
};

template<>
struct Converter<float> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    float val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     float* out);
};

template<>
struct Converter<double> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    double val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     double* out);
};

template<>
struct Converter<std::string> {
  // This crashes when val.size() > v8::String::kMaxLength.
  static v8::Local<v8::String> ToV8(v8::Isolate* isolate,
                                    const std::string& val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     std::string* out);
};

template <>
struct Converter<v8::Local<v8::Function>> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                   v8::Local<v8::Function> val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     v8::Local<v8::Function>* out);
};

template<>
struct Converter<v8::Local<v8::Object> > {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    v8::Local<v8::Object> val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     v8::Local<v8::Object>* out);
};

template <>
struct Converter<v8::Local<v8::Promise>> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                   v8::Local<v8::Promise> val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     v8::Local<v8::Promise>* out);
};

template<>
struct Converter<v8::Local<v8::ArrayBuffer> > {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    v8::Local<v8::ArrayBuffer> val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     v8::Local<v8::ArrayBuffer>* out);
};

template<>
struct Converter<v8::Local<v8::External> > {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    v8::Local<v8::External> val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     v8::Local<v8::External>* out);
};

template<>
struct Converter<v8::Local<v8::Value> > {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                    v8::Local<v8::Value> val);
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     v8::Local<v8::Value>* out);
};

template<typename T>
struct Converter<std::vector<T> > {
  static std::conditional_t<ToV8ReturnsMaybe<T>::value,
                            v8::MaybeLocal<v8::Value>,
                            v8::Local<v8::Value>>
  ToV8(v8::Isolate* isolate, const std::vector<T>& val) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Array> result(
        v8::Array::New(isolate, static_cast<int>(val.size())));
    for (uint32_t i = 0; i < val.size(); ++i) {
      v8::MaybeLocal<v8::Value> maybe = Converter<T>::ToV8(isolate, val[i]);
      v8::Local<v8::Value> element;
      if (!maybe.ToLocal(&element))
        return {};
      bool property_created;
      if (!result->CreateDataProperty(context, i, element)
               .To(&property_created) ||
          !property_created) {
        std::cout << "CreateDataProperty should always succeed here.";
      }
    }
    return result;
  }

  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     std::vector<T>* out) {
    if (!val->IsArray())
      return false;

    std::vector<T> result;
    v8::Local<v8::Array> array(v8::Local<v8::Array>::Cast(val));
    uint32_t length = array->Length();
    for (uint32_t i = 0; i < length; ++i) {
      v8::Local<v8::Value> v8_item;
      if (!array->Get(isolate->GetCurrentContext(), i).ToLocal(&v8_item))
        return false;
      T item;
      if (!Converter<T>::FromV8(isolate, v8_item, &item))
        return false;
      result.push_back(item);
    }

    out->swap(result);
    return true;
  }
};

template<typename T>
struct ToV8ReturnsMaybe<std::vector<T>> {
  static const bool value = ToV8ReturnsMaybe<T>::value;
};

// Convenience functions that deduce T.
template <typename T>
std::conditional_t<ToV8ReturnsMaybe<T>::value,
                   v8::MaybeLocal<v8::Value>,
                   v8::Local<v8::Value>>
ConvertToV8(v8::Isolate* isolate, const T& input) {
  return Converter<T>::ToV8(isolate, input);
}

template <typename T>
std::enable_if_t<ToV8ReturnsMaybe<T>::value, bool> TryConvertToV8(
    v8::Isolate* isolate,
    const T& input,
    v8::Local<v8::Value>* output) {
  return ConvertToV8(isolate, input).ToLocal(output);
}

template <typename T>
std::enable_if_t<!ToV8ReturnsMaybe<T>::value, bool> TryConvertToV8(
    v8::Isolate* isolate,
    const T& input,
    v8::Local<v8::Value>* output) {
  *output = ConvertToV8(isolate, input);
  return true;
}

template<typename T>
bool ConvertFromV8(v8::Isolate* isolate, v8::Local<v8::Value> input,
                   T* result) {
  return Converter<T>::FromV8(isolate, input, result);
}

std::string V8ToString(v8::Isolate* isolate,
                      v8::Local<v8::Value> value);

// This crashes when input.size() > v8::String::kMaxLength.
inline v8::Local<v8::String> StringToV8(
    v8::Isolate* isolate,
    const std::string& input) {
  return ConvertToV8(isolate, input).As<v8::String>();
}

}  // namespace nica

#endif  // CORE_CONVERTER_H_
