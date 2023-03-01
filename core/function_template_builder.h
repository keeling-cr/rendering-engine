#ifndef CORE_FUNCTION_TEMPLATE_BUILDER_H_
#define CORE_FUNCTION_TEMPLATE_BUILDER_H_

#include <type_traits>
#include <utility>

#include "core/converter.h"
#include "core/function_template.h"
#include "v8-forward.h"

namespace nica {

namespace internal {

template <typename T>
v8::Local<v8::FunctionTemplate> CreateFunctionTemplateForFunction(v8::Isolate* isolate,
                                                       T callback,
                                                       const char* type_name) {
  InvokerOptions options;
  // todo(liqining): Suppoer non-member function using
  // std::is_member_function_pointer
  options.holder_is_first_argument = true;
  options.holder_type = type_name;
  return ::nica::CreateFunctionTemplate(
      isolate, base::BindRepeating(std::move(callback)), std::move(options));
}

}  // namespace internal

class FunctionTemplateBuilder {
  public:
    explicit FunctionTemplateBuilder(v8::Isolate* isolate, v8::FunctionCallback construct_callback);
    FunctionTemplateBuilder(v8::Isolate* isolate, v8::FunctionCallback construct_callback, const char* type_name);
    FunctionTemplateBuilder(v8::Isolate* isolate,
                        const char* type_name,
                        v8::Local<v8::FunctionTemplate> tmpl);
    FunctionTemplateBuilder(const FunctionTemplateBuilder& other);
    ~FunctionTemplateBuilder();

    v8::Local<v8::FunctionTemplate> Build();
    void SetClassName(const std::string& class_name);

    template<typename T>
    FunctionTemplateBuilder& SetMethod(const base::StringPiece& name,
                                   const T& callback) {
        return SetImpl(
            name, internal::CreateFunctionTemplateForFunction(isolate_, callback, type_name_));
    }

    template<typename T, typename U>
    FunctionTemplateBuilder& SetProperty(const base::StringPiece& name,
                                  const T& getter, const U& setter) {
      return SetPropertyImpl(name, internal::CreateFunctionTemplateForFunction(isolate_, getter, type_name_),
        internal::CreateFunctionTemplateForFunction(isolate_, setter, type_name_));
    }

    template<typename T>
    FunctionTemplateBuilder& SetValue(const base::StringPiece& name, T val) {
      return SetImpl(name, ConvertToV8(isolate_, val));
    }
  
  private:
    FunctionTemplateBuilder& SetImpl(const base::StringPiece& name,
                            v8::Local<v8::Data> val);
    FunctionTemplateBuilder& SetPropertyImpl(
      const base::StringPiece& name, v8::Local<v8::FunctionTemplate> getter,
      v8::Local<v8::FunctionTemplate> setter);

    v8::Isolate* isolate_ = nullptr;
    const char* type_name_ = nullptr;
    v8::Local<v8::FunctionTemplate> template_;
};
}
#endif