#ifndef CORE_OBJECT_TEMPLATE_BUILDER_H_
#define CORE_OBJECT_TEMPLATE_BUILDER_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "base/strings/string_piece.h"
#include "core/converter.h"
#include "core/function_template.h"
#include "v8-forward.h"

namespace nica {

namespace internal {

template <typename T>
v8::Local<v8::FunctionTemplate> CreateFunctionTemplate(v8::Isolate* isolate,
                                                       std::function<T> callback,
                                                       const char* type_name) {
  InvokerOptions options;
  // todo(liqining): Suppoer non-member function using
  // std::is_member_function_pointer
  options.holder_is_first_argument = true;
  options.holder_type = type_name;
  return ::nica::CreateFunctionTemplate(
      isolate, std::move(callback), std::move(options));
}

}  // namespace internal

class ObjectTemplateBuilder {
 public:
  explicit ObjectTemplateBuilder(v8::Isolate* isolate);
  ObjectTemplateBuilder(v8::Isolate* isolate, const char* type_name);
  ObjectTemplateBuilder(v8::Isolate* isolate,
                        const char* type_name,
                        v8::Local<v8::ObjectTemplate> tmpl);
  ObjectTemplateBuilder(const ObjectTemplateBuilder& other);
  ~ObjectTemplateBuilder();

  template<typename T>
  ObjectTemplateBuilder& SetValue(const base::StringPiece& name, T val) {
    return SetImpl(name, ConvertToV8(isolate_, val));
  }

  template<typename T>
  ObjectTemplateBuilder& SetMethod(const base::StringPiece& name,
                                   const T& callback) {
    return SetImpl(
        name, internal::CreateFunctionTemplate(isolate_, callback, type_name_));
  }

  template <typename T>
  ObjectTemplateBuilder& SetMethod(v8::Local<v8::Name> name,
                                   const T& callback) {
    return SetImpl(
        name, internal::CreateFunctionTemplate(isolate_, callback, type_name_));
  }

  template<typename T>
  ObjectTemplateBuilder& SetProperty(const base::StringPiece& name,
                                     const T& getter) {
    return SetPropertyImpl(
        name, internal::CreateFunctionTemplate(isolate_, getter, type_name_),
        v8::Local<v8::FunctionTemplate>());
  }
  template<typename T, typename U>
  ObjectTemplateBuilder& SetProperty(const base::StringPiece& name,
                                     const T& getter, const U& setter) {
    return SetPropertyImpl(
        name, internal::CreateFunctionTemplate(isolate_, getter, type_name_),
        internal::CreateFunctionTemplate(isolate_, setter, type_name_));
  }

  template <typename T>
  ObjectTemplateBuilder& SetLazyDataProperty(const base::StringPiece& name,
                                             const T& getter) {
    InvokerOptions options;
    if (std::is_member_function_pointer<T>::value) {
      options.holder_is_first_argument = true;
      options.holder_type = type_name_;
    }
    v8::AccessorNameGetterCallback callback;
    v8::Local<v8::Value> data;
    std::tie(callback, data) = CreateDataPropertyCallback(
        isolate_, std::move(getter), std::move(options));
    return SetLazyDataPropertyImpl(name, callback, data);
  }

  ObjectTemplateBuilder& AddNamedPropertyInterceptor();
  ObjectTemplateBuilder& AddIndexedPropertyInterceptor();

  v8::Local<v8::ObjectTemplate> Build();

 private:
  ObjectTemplateBuilder& SetImpl(const base::StringPiece& name,
                                 v8::Local<v8::Data> val);
  ObjectTemplateBuilder& SetImpl(v8::Local<v8::Name> name,
                                 v8::Local<v8::Data> val);
  ObjectTemplateBuilder& SetPropertyImpl(
      const base::StringPiece& name, v8::Local<v8::FunctionTemplate> getter,
      v8::Local<v8::FunctionTemplate> setter);
  ObjectTemplateBuilder& SetLazyDataPropertyImpl(
      const base::StringPiece& name,
      v8::AccessorNameGetterCallback callback,
      v8::Local<v8::Value> data);

  v8::Isolate* isolate_;

  const char* type_name_ = nullptr;

  v8::Local<v8::ObjectTemplate> template_;
};

}  // namespace gin

#endif  // GIN_OBJECT_TEMPLATE_BUILDER_H_
