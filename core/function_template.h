#ifndef CORE_FUNCTION_TEMPLATE_H_
#define CORE_FUNCTION_TEMPLATE_H_

#include <stddef.h>
#include <utility>

#include <functional>

#include "base/check.h"
// todo(liqining):
// #include "base/strings/strcat.h"
#include "core/arguments.h"
#include "core/converter.h"
#include "v8-external.h"
#include "v8-forward.h"
#include "v8-persistent-handle.h"
#include "v8-template.h"

namespace nica {

struct InvokerOptions {
  bool holder_is_first_argument = false;
  const char* holder_type = nullptr;  // Null if unknown or not applicable.
};

namespace internal {

template<typename T>
struct CallbackParamTraits {
  typedef T LocalType;
};
template<typename T>
struct CallbackParamTraits<const T&> {
  typedef T LocalType;
};
template<typename T>
struct CallbackParamTraits<const T*> {
  typedef T* LocalType;
};

// This simple base class is used so that we can share a single object template
// among every CallbackHolder instance.
class CallbackHolderBase {
 public:
  CallbackHolderBase(const CallbackHolderBase&) = delete;
  CallbackHolderBase& operator=(const CallbackHolderBase&) = delete;

  v8::Local<v8::External> GetHandle(v8::Isolate* isolate);

 protected:
  explicit CallbackHolderBase(v8::Isolate* isolate);
  virtual ~CallbackHolderBase();

 private:
  static void FirstWeakCallback(
      const v8::WeakCallbackInfo<CallbackHolderBase>& data);
  static void SecondWeakCallback(
      const v8::WeakCallbackInfo<CallbackHolderBase>& data);

  v8::Global<v8::External> v8_ref_;
};

template<typename Sig>
class CallbackHolder : public CallbackHolderBase {
 public:
  CallbackHolder(v8::Isolate* isolate,
                 Sig callback,
                 InvokerOptions invoker_options)
      : CallbackHolderBase(isolate),
        callback(std::move(callback)),
        invoker_options(std::move(invoker_options)) {}
  CallbackHolder(const CallbackHolder&) = delete;
  CallbackHolder& operator=(const CallbackHolder&) = delete;

  Sig callback;
  InvokerOptions invoker_options;

 private:
  ~CallbackHolder() override = default;
};

template <typename T>
bool GetNextArgument(Arguments* args,
                     const InvokerOptions& invoker_options,
                     bool is_first,
                     T* result) {
  if (is_first && invoker_options.holder_is_first_argument) {
    return args->GetHolder(result);
  } else {
    return args->GetNext(result);
  }
}

// For advanced use cases, we allow callers to request the unparsed Arguments
// object and poke around in it directly.
inline bool GetNextArgument(Arguments* args,
                            const InvokerOptions& invoker_options,
                            bool is_first,
                            Arguments* result) {
  *result = *args;
  return true;
}
inline bool GetNextArgument(Arguments* args,
                            const InvokerOptions& invoker_options,
                            bool is_first,
                            Arguments** result) {
  *result = args;
  return true;
}

// It's common for clients to just need the isolate, so we make that easy.
inline bool GetNextArgument(Arguments* args,
                            const InvokerOptions& invoker_options,
                            bool is_first,
                            v8::Isolate** result) {
  *result = args->isolate();
  return true;
}

// Throws an error indicating conversion failure.
void ThrowConversionError(Arguments* args,
                                     const InvokerOptions& invoker_options,
                                     size_t index);

// Class template for extracting and storing single argument for callback
// at position |index|.
template <size_t index, typename ArgType>
struct ArgumentHolder {
  using ArgLocalType = typename CallbackParamTraits<ArgType>::LocalType;

  ArgLocalType value;
  bool ok;

  ArgumentHolder(Arguments* args, const InvokerOptions& invoker_options)
      : ok(GetNextArgument(args, invoker_options, index == 0, &value)) {
    if (!ok)
      ThrowConversionError(args, invoker_options, index);
  }
};

// Class template for converting arguments from JavaScript to C++ and running
// the callback with them.
template <typename IndicesType, typename... ArgTypes>
class Invoker;

template <size_t... indices, typename... ArgTypes>
class Invoker<std::index_sequence<indices...>, ArgTypes...>
    : public ArgumentHolder<indices, ArgTypes>... {
 public:
  // Invoker<> inherits from ArgumentHolder<> for each argument.
  // C++ has always been strict about the class initialization order,
  // so it is guaranteed ArgumentHolders will be initialized (and thus, will
  // extract arguments from Arguments) in the right order.
  Invoker(Arguments* args, const InvokerOptions& invoker_options)
      : ArgumentHolder<indices, ArgTypes>(args, invoker_options)...,
        args_(args) {}

  bool IsOK() {
    return And(ArgumentHolder<indices, ArgTypes>::ok...);
  }

  template <typename ReturnType>
  void DispatchToCallback(std::function<ReturnType(ArgTypes...)> callback) {
    args_->Return(
      std::invoke(callback, std::move(ArgumentHolder<indices, ArgTypes>::value)...));
  }

  // In C++, you can declare the function foo(void), but you can't pass a void
  // expression to foo. As a result, we must specialize the case of Callbacks
  // that have the void return type.
  void DispatchToCallback(std::function<void(ArgTypes...)> callback) {
    std::invoke(callback, std::move(ArgumentHolder<indices, ArgTypes>::value)...);
  }

 private:
  static bool And() { return true; }
  template <typename... T>
  static bool And(bool arg1, T... args) {
    return arg1 && And(args...);
  }

  Arguments* args_;
};

// DispatchToCallback converts all the JavaScript arguments to C++ types and
// invokes the base::RepeatingCallback.
template <typename Sig>
struct Dispatcher {};

template <typename ReturnType, typename... ArgTypes>
struct Dispatcher<std::function<ReturnType(ArgTypes...)>> {
  static void DispatchToCallbackImpl(Arguments* args) {
    v8::Local<v8::External> v8_holder;
    CHECK(args->GetData(&v8_holder));
    CallbackHolderBase* holder_base = reinterpret_cast<CallbackHolderBase*>(
        v8_holder->Value());

    typedef CallbackHolder<std::function<ReturnType(ArgTypes...)>> HolderT;
    HolderT* holder = static_cast<HolderT*>(holder_base);

    using Indices = std::index_sequence_for<ArgTypes...>;
    Invoker<Indices, ArgTypes...> invoker(args, holder->invoker_options);
    if (invoker.IsOK())
      invoker.DispatchToCallback(holder->callback);
  }

  static void DispatchToCallback(
      const v8::FunctionCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    DispatchToCallbackImpl(&args);
  }

  static void DispatchToCallbackForProperty(
      v8::Local<v8::Name>,
      const v8::PropertyCallbackInfo<v8::Value>& info) {
    Arguments args(info);
    DispatchToCallbackImpl(&args);
  }
};

}  // namespace internal

template <typename Sig>
v8::Local<v8::FunctionTemplate> CreateFunctionTemplate(
    v8::Isolate* isolate,
    Sig callback,
    InvokerOptions invoker_options = {}) {
  typedef internal::CallbackHolder<Sig> HolderT;
  HolderT* holder =
      new HolderT(isolate, std::move(callback), std::move(invoker_options));

  v8::Local<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New(
      isolate, &internal::Dispatcher<Sig>::DispatchToCallback,
      ConvertToV8<v8::Local<v8::External>>(isolate, holder->GetHandle(isolate)),
      v8::Local<v8::Signature>(), 0, v8::ConstructorBehavior::kThrow);
  return tmpl;
}

template <typename Sig>
std::pair<v8::AccessorNameGetterCallback, v8::Local<v8::Value>>
CreateDataPropertyCallback(v8::Isolate* isolate,
                           Sig callback,
                           InvokerOptions invoker_options = {}) {
  typedef internal::CallbackHolder<Sig> HolderT;
  HolderT* holder =
      new HolderT(isolate, std::move(callback), std::move(invoker_options));
  return {&internal::Dispatcher<Sig>::DispatchToCallbackForProperty,
          ConvertToV8<v8::Local<v8::External>>(isolate,
                                               holder->GetHandle(isolate))};
}

}  // namespace nica

#endif  // CORE_FUNCTION_TEMPLATE_H_
