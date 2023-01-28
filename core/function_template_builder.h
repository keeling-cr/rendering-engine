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
  private:
    FunctionTemplateBuilder& SetImpl(const base::StringPiece& name,
                            v8::Local<v8::Data> val);
    v8::Isolate* isolate_;
    const char* type_name_ = nullptr;
    v8::Local<v8::FunctionTemplate> template_;
};
}
#endif