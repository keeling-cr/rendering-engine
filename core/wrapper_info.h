#ifndef CORE_PUBLIC_WRAPPER_INFO_H_
#define CORE_PUBLIC_WRAPPER_INFO_H_

#include "core/embedders.h"
#include "v8-forward.h"

namespace nica {

enum InternalFields {
  kWrapperInfoIndex,
  kEncodedValueIndex,
  kNumberOfInternalFields,
};

struct WrapperInfo {
  static WrapperInfo* From(v8::Local<v8::Object> object);
  const Embedder embedder;
};

}  // namespace nica

#endif  // GIN_PUBLIC_WRAPPER_INFO_H_
