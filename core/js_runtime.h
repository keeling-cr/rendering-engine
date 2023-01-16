#ifndef CORE_JS_RUNTIME_H_
#define CORE_JS_RUNTIME_H_

#include <unordered_map>
#include <cstdio>

#include "base/files/file_path.h"
#include "libplatform/libplatform.h"
#include "v8.h"

namespace nica {

class JSContext;
class JSIsolate;

class JSRuntime {
    public: 
      JSRuntime();
      ~JSRuntime();
      JSRuntime(const JSRuntime&) = delete;
      JSRuntime& operator=(const JSRuntime&) = delete;

      void EvaluateJavascriptSource(const std::string& source);
      void EvaluateJavascriptFile(const base::FilePath& js_file_path);
      JSContext* GetJSContext();
      JSIsolate* GetJSIsolate();

      class Scope {
         public:
            explicit Scope(JSRuntime* runtime);
            Scope(const Scope&) = delete;
            Scope& operator=(const Scope&) = delete;
            ~Scope();

         private:
            v8::Isolate::Scope isolate_scope_;
            v8::HandleScope handle_scope_;
            v8::Context::Scope scope_;
      };

    private:
      friend class Scope;

      void CreateJSContext();

      std::unique_ptr<JSIsolate> js_isolate_;
      std::unique_ptr<JSContext> js_context_;
      std::unique_ptr<v8::Platform> platform_;
};
} // namespace nica

#endif // CORE_JS_RUNTIME_H_
