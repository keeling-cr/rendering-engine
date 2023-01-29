#include "core/function_template_builder.h"
#include "core/wrapper_info.h"

namespace nica {

namespace {

v8::Local<v8::FunctionTemplate> CreateConstructorTemplate(
    v8::Isolate* isolate, const char* class_name, v8::FunctionCallback callback) {
  v8::Local<v8::FunctionTemplate> constructor = v8::FunctionTemplate::New(isolate, callback);
  constructor->SetClassName(nica::StringToSymbol(isolate, class_name));
  constructor->ReadOnlyPrototype();
  v8::Local<v8::ObjectTemplate> instance = constructor->InstanceTemplate();
  instance->SetInternalFieldCount(kNumberOfInternalFields);
  return constructor;
}

} // namespace 

FunctionTemplateBuilder::FunctionTemplateBuilder(
    v8::Isolate* isolate, v8::FunctionCallback construct_callback)
    : FunctionTemplateBuilder(
        isolate, construct_callback, nullptr) {}

FunctionTemplateBuilder::FunctionTemplateBuilder(
    v8::Isolate* isolate, v8::FunctionCallback construct_callback, const char* type_name)
    : FunctionTemplateBuilder(isolate,
                            type_name,
                            CreateConstructorTemplate(isolate, type_name, construct_callback)) {}

FunctionTemplateBuilder::FunctionTemplateBuilder(v8::Isolate* isolate,
                                             const char* type_name,
                                             v8::Local<v8::FunctionTemplate> tmpl)
    : isolate_(isolate), type_name_(type_name), template_(tmpl) {
    template_->SetClassName(nica::StringToSymbol(isolate_, type_name));
    template_->ReadOnlyPrototype();
}

FunctionTemplateBuilder::FunctionTemplateBuilder(
    const FunctionTemplateBuilder& other) = default;

FunctionTemplateBuilder::~FunctionTemplateBuilder() = default;

void FunctionTemplateBuilder::SetClassName(const std::string& class_name) {
    template_->SetClassName(nica::StringToSymbol(isolate_, class_name));
}

FunctionTemplateBuilder& FunctionTemplateBuilder::SetImpl(
    const base::StringPiece& name,
    v8::Local<v8::Data> val) {
    template_->PrototypeTemplate()->Set(StringToSymbol(isolate_, name), val);
    return *this;
}

FunctionTemplateBuilder& FunctionTemplateBuilder::SetPropertyImpl(
    const base::StringPiece& name,
    v8::Local<v8::FunctionTemplate> getter,
    v8::Local<v8::FunctionTemplate> setter) {
   template_->InstanceTemplate()->SetAccessorProperty(StringToSymbol(isolate_, name), getter, setter);
   return *this;
}

v8::Local<v8::FunctionTemplate> FunctionTemplateBuilder::Build() {
    v8::Local<v8::FunctionTemplate> result = template_;
    template_.Clear();
    return result;
} 

}