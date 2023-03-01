#include "binding/modules/test/bind_test.h"

#include "functional"

#include "core/handle.h"
#include "core/js_isolate.h"
#include "core/js_context.h"
#include "core/object_template_builder.h"
#include "v8-context.h"

namespace bind {

nica::WrapperInfo BindTest::kWrapperInfo = {
    nica::kEmbedderNicaMain};

BindTest::BindTest() = default;

BindTest::~BindTest() = default;

void BindTest::TestBind() {
    LOG(ERROR) << "keilingnica " << __func__;
}

int BindTest::TestBindWithReturn() {
    LOG(ERROR) << "keilingnica " << __func__;
    return 100;
}

int BindTest::TestBindWithParams(int a, int b) {
   LOG(ERROR) << "keilingnica " << __func__; 
   return a + b;
}

nica::ObjectTemplateBuilder 
BindTest::GetObjectTemplateBuilder(
    v8::Isolate* isolate) {
   return nica::Wrappable<BindTest>::GetObjectTemplateBuilder(isolate)
        .SetMethod("test", &BindTest::TestBind)
        .SetMethod("testwithreturn", &BindTest::TestBindWithReturn)
        .SetMethod("testwithparam", &BindTest::TestBindWithParams);
}

void BindTest::Register(
        nica::JSIsolate* js_isolate, nica::JSContext* js_context) {
    v8::Isolate* isolate = js_isolate->isolate();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = js_context->context();
    v8::Context::Scope context_scope(context);

    nica::Handle<BindTest> bind_test = nica::CreateHandle(
        isolate, new BindTest());
    if (bind_test.IsEmpty())
        return;

    v8::Local<v8::Object> global = context->Global();
    global
        ->Set(context, nica::StringToV8(isolate, "bindTest"),
            bind_test.ToV8())
        .ToChecked();    
}

} // namespace bind