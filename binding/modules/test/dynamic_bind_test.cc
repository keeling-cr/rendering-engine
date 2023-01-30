#include "binding/modules/test/dynamic_bind_test.h"

#include "functional"

#include "core/handle.h"
#include "core/js_isolate.h"
#include "core/js_context.h"
#include "core/function_template_builder.h"
#include "v8-context.h"

namespace bind {

namespace {

void DynamicBindTestNew(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);

    if (!args.IsConstructCall()) {
        isolate->ThrowError("Canvas must be constructed with new");
        return;
    }
    DynamicBindTest* test = new DynamicBindTest(isolate, args.Holder());

    // // return the new object back to the javascript caller
    args.GetReturnValue().Set(args.Holder()); 
}

} // namespace 

nica::WrapperInfo DynamicBindTest::kWrapperInfo = {
    nica::kEmbedderNicaMain};

DynamicBindTest::DynamicBindTest(
    v8::Isolate* isolate, v8::Local<v8::Object> instance)
    : V8Object(isolate, instance) {}

DynamicBindTest::~DynamicBindTest() = default;

void DynamicBindTest::TestBind() {
    LOG(ERROR) << "keilingnica " << __func__;
}

int DynamicBindTest::TestBindWithReturn() {
    LOG(ERROR) << "keilingnica " << __func__;
    return 100;
}

int DynamicBindTest::TestBindWithParams(int a, int b) {
   LOG(ERROR) << "keilingnica " << __func__; 
   return a + b;
}

void DynamicBindTest::SetWidth(int a) {
   LOG(ERROR) << "keilingnica " << __func__; 
}

int DynamicBindTest::GetWidth() {
   LOG(ERROR) << "keilingnica " << __func__; 
   return 77;
}

nica::FunctionTemplateBuilder 
DynamicBindTest::GetFunctionTemplateBuilder(
    v8::Isolate* isolate) {
    return nica::FunctionTemplateBuilder(isolate, DynamicBindTestNew)
            .SetMethod("test", std::function<void(DynamicBindTest*)>{&DynamicBindTest::TestBind})
            .SetMethod("testwithreturn", std::function<int(DynamicBindTest*)>{&DynamicBindTest::TestBindWithReturn})
            .SetMethod("testwithparam", std::function<int(DynamicBindTest*, int, int)>{&DynamicBindTest::TestBindWithParams})
            .SetProperty("width", std::function<int(DynamicBindTest*)>{&DynamicBindTest::GetWidth}, std::function<void(DynamicBindTest*, int)>{&DynamicBindTest::SetWidth});
}

} // namespace bind