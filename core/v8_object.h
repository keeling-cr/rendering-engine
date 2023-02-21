#ifndef CORE_V8_OBJECT_H_
#define CORE_V8_OBJECT_H_

#include "core/function_template.h"
#include "core/js_context.h"
#include "core/js_isolate.h"
#include "core/per_isolate_data.h"
#include "core/wrapper_info.h"
#include "v8-forward.h"
#include "v8.h"

namespace nica {

class V8ObjectBase {
  public:
    inline static V8ObjectBase* FromV8Object(
        v8::Local<v8::Value> val, WrapperInfo* wrapper_info) {
        if (!val->IsObject())
            return nullptr;
        
        v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(val);
        WrapperInfo* info = WrapperInfo::From(obj);
        if (!info)
            return nullptr;
        
        if (info != wrapper_info)
            return nullptr;
        return static_cast<V8ObjectBase*>(
            obj->GetAlignedPointerFromInternalField(kEncodedValueIndex));
    }

    inline v8::Local<v8::Object> GetV8Object() {
      return v8::Local<v8::Object>::New(isolate_, instance_);
    }

  protected:
    V8ObjectBase(v8::Isolate* isolate): isolate_(isolate) {}
    virtual ~V8ObjectBase();

    void SetInstance(
        v8::Isolate* isolate, v8::Handle<v8::Object> instance, WrapperInfo* info, bool weak = false);
    v8::Isolate* GetIsolate() { return isolate_; }
  private:
    V8ObjectBase(const V8ObjectBase&);
    V8ObjectBase& operator = (const V8ObjectBase&);

    v8::Persistent<v8::Object> instance_;
    v8::Isolate* isolate_;

    static void WeakCallback(v8::Persistent<v8::Value> value, void* data);
};

template<class T>
class ConstructorMode {
  public:
    ConstructorMode() { s_allow_construction = true; }
    ~ConstructorMode() { s_allow_construction = false; }
    static bool IsConstructionAllowed() { return s_allow_construction; }
  private:
    static bool s_allow_construction;
};
template<class T>
bool ConstructorMode<T>::s_allow_construction = false;


template<class T>
class V8Object : public V8ObjectBase {
  public:
    static void Register(JSIsolate* js_isolate, JSContext* js_context) {
        v8::Isolate* isolate = js_isolate->isolate();
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = js_context->context();
        v8::Context::Scope context_scope(context);

        PerIsolateData* data = PerIsolateData::From(isolate);
        v8::Local<v8::FunctionTemplate> templ = 
            data->GetFunctionTemplate(&T::kWrapperInfo);

        if (templ.IsEmpty()) {
            templ = T::GetFunctionTemplateBuilder(isolate).Build();
            data->SetFunctionTemplate(&T::kWrapperInfo, templ);
        }

        v8::Local<v8::Object> global = context->Global();
        global
            ->Set(context, nica::StringToV8(isolate, T::ClassName()),
            templ->GetFunction(context).ToLocalChecked()).ToChecked();    
    }

  protected:
    V8Object(v8::Isolate* isolate, 
        v8::Handle<v8::Object> instance = v8::Local<v8::Object>())
        : V8ObjectBase(isolate) {
        if (instance.IsEmpty()) {
            ConstructorMode<T> mode;
            instance = Create(isolate);
        }
        SetInstance(isolate, instance, &T::kWrapperInfo);
    }

    static v8::Handle<v8::Object> Create(v8::Isolate* isolate) {
        PerIsolateData* data = PerIsolateData::From(isolate);
        v8::Local<v8::FunctionTemplate> templ = data->GetFunctionTemplate(&T::kWrapperInfo);
        return templ->GetFunction(
            isolate->GetCurrentContext()).ToLocalChecked()
            ->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    }  
};

template <typename T>
struct ToV8ReturnsMaybe<
    T*,
    typename std::enable_if<
        std::is_convertible<T*, V8ObjectBase*>::value>::type> {
    static const bool value = true;
};

template <typename T>
struct Converter<T*,
            typename std::enable_if<
                        std::is_convertible<T*, V8ObjectBase*>::value>::type> {
  static v8::MaybeLocal<v8::Value> ToV8(v8::Isolate* isolate, T* val) {
    if (val == nullptr)
      return v8::Null(isolate);
    
         v8::Local<v8::Object> instance = val->GetV8Object();
    return v8::MaybeLocal<v8::Value>(instance);
  }

  static bool FromV8(v8::Isolate* isolate, v8::Local<v8::Value> val, T** out) {
    *out = static_cast<T*>(V8ObjectBase::FromV8Object(val, &T::kWrapperInfo));
    return true;
  }
};

} // namespace nica

#endif // CORE_V8_OBJECT_H_