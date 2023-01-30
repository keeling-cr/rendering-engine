#ifndef CORE_JS_CONTEXT_H_
#define CORE_JS_CONTEXT_H_

#include "v8.h"

namespace nica {

class JSContext {
    public: 
        explicit JSContext(v8::Isolate* isolate);
        JSContext(const JSContext&) = delete;
        JSContext& operator=(const JSContext&) = delete;
        ~JSContext();
        
        static std::unique_ptr<JSContext> Create(v8::Isolate* isolate);

        v8::Isolate* isolate() const { return isolate_; }
        
        v8::Local<v8::Context> context() const {
            return v8::Local<v8::Context>::New(isolate_, context_);
        }

        void SetContext(v8::Local<v8::Context> context);

    private:
        v8::Isolate* isolate_ = nullptr;
        v8::UniquePersistent<v8::Context> context_;
};
} // namespace nica

#endif // CORE_JS_CONTEXT_H_
