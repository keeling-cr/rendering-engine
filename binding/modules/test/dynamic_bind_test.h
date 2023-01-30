#ifndef BINDING_MODULES_TEST_DYNAMIC_BIND_TEST_H_ 
#define BINDING_MODULES_TEST_DYNAMIC_BIND_TEST_H_

#include "core/arguments.h"
#include "core/wrappable.h"
#include "core/v8_object.h"

namespace nica {
class JSIsolate;
class JSContext;
class FunctionTemplateBuilder;
}

namespace bind {
class DynamicBindTest : public nica::V8Object<DynamicBindTest> {
 public:
 
    static nica::WrapperInfo kWrapperInfo;
    static const char* ClassName() { return "DynamicBindTest"; }
    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
        v8::Isolate* isolate);

    DynamicBindTest(v8::Isolate* isolate, v8::Local<v8::Object> instance);
    ~DynamicBindTest() override;
    DynamicBindTest(const DynamicBindTest&) = delete;
    DynamicBindTest& operator=(const DynamicBindTest&) = delete;

  private:
    void TestBind();
    int TestBindWithReturn();
    int TestBindWithParams(int a, int b);
    void SetWidth(int a);
    int GetWidth();
};
}

#endif  // BINDING_MODULES_TEST_DYNAMIC_BIND_TEST_H_
