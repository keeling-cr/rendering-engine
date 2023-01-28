#ifndef BINDING_MODULES_TEST_DYNAMIC_BIND_TEST_H_ 
#define BINDING_MODULES_TEST_DYNAMIC_BIND_TEST_H_

#include "core/arguments.h"
#include "core/wrappable.h"

namespace nica {
class JSIsolate;
class JSContext;
class FunctionTemplateBuilder;
}

namespace bind {
class DynamicBindTest : public nica::Wrappable<DynamicBindTest> {
 public:
 
    static nica::WrapperInfo kWrapperInfo;
    static const bool is_dynamic_obj = true;
    DynamicBindTest();
    ~DynamicBindTest() override;
    DynamicBindTest(const DynamicBindTest&) = delete;
    DynamicBindTest& operator=(const DynamicBindTest&) = delete;

    static void Register(
        nica::JSIsolate* js_isolate, nica::JSContext* js_contrxt);
  private:

    static nica::FunctionTemplateBuilder GetFunctionTemplateBuilder(
        v8::Isolate* isolate);
    
    const char* GetTypeName() final { return "DynamicBindTest"; }
    
    void TestBind();
    int TestBindWithReturn();
    int TestBindWithParams(int a, int b);
};
}

#endif  // BINDING_MODULES_TEST_DYNAMIC_BIND_TEST_H_
