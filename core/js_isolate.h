#ifndef CORE_JS_ISOLATE_H_
#define CORE_JS_ISOLATE_H_

#include <memory>
#include <cstdio>

#include "v8-isolate.h"
namespace nica {

class PerIsolateData;

class JSIsolate {
  public:
    JSIsolate();
    JSIsolate(const JSIsolate&) = delete;
    JSIsolate& operator=(const JSIsolate&) = delete;
    ~JSIsolate();

    static std::unique_ptr<JSIsolate> Create();

    v8::Isolate* isolate() { return isolate_; }

  private:
    v8::Isolate* isolate_ = nullptr;
    std::unique_ptr<PerIsolateData> isolate_data_;
};

}

#endif // CORE_JS_ISOLATE_H_