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

  BindTest(const BindTest&) = delete;
  BindTest& operator=(const BindTest&) = delete;

  static void Register(
    nica::JSIsolate* js_isolate, nica::JSContext* js_contrxt);
 private:
  BindTest();
  ~BindTest() override;

  // nica::WrappableBase
  nica::ObjectTemplateBuilder GetObjectTemplateBuilder(
      v8::Isolate* isolate) override;
  
  void TestBind();
  int TestBindWithReturn();
  int TestBindWithParams(int a, int b);
};
}

#endif  // BINDING_MODULES_TEST_BIND_TEST_H_
