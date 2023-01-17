#include "core/wrapper_info.h"
#include "v8-object.h"

namespace nica {

WrapperInfo* WrapperInfo::From(v8::Local<v8::Object> object) {
  if (object->InternalFieldCount() != kNumberOfInternalFields)
    return NULL;
  WrapperInfo* info = static_cast<WrapperInfo*>(
      object->GetAlignedPointerFromInternalField(kWrapperInfoIndex));
  return info->embedder == kEmbedderNicaMain ? info : NULL;
}

}  // namespace nica
