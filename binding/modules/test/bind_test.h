#ifndef BINDING_MODULES_TEST_BIND_TEST_H_ 
#define BINDING_MODULES_TEST_BIND_TEST_H_

#include "core/arguments.h"
#include "core/wrappable.h"

namespace nica {
class JSIsolate;
class JSContext;
}

namespace bind {
class BindTest : public nica::Wrappable<BindTest> {
 public:
  static nica::WrapperInfo kWrapperInfo;
  static const bool is_dynamic_obj = false;

  BindTest();
  ~BindTest() override;
  BindTest(const BindTest&) = delete;
  BindTest& operator=(const BindTest&) = delete;

  static void Register(
    nica::JSIsolate* js_isolate, nica::JSContext* js_contrxt);
 private:

  // nica::WrappableBase
  nica::ObjectTemplateBuilder GetObjectTemplateBuilder(
      v8::Isolate* isolate) final;
  
  const char* GetTypeName() final { return "BindTest"; }
  
  void TestBind();
  int TestBindWithReturn();
  int TestBindWithParams(int a, int b);
};
}

#endif  // BINDING_MODULES_TEST_BIND_TEST_H_
