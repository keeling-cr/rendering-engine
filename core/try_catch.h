#ifndef CORE_TRY_CATCH_H_
#define CORE_TRY_CATCH_H_

#include <string>

#include "v8-exception.h"

namespace nica {

class TryCatch {
 public:
  explicit TryCatch(v8::Isolate* isolate);
  TryCatch(const TryCatch&) = delete;
  TryCatch& operator=(const TryCatch&) = delete;
  ~TryCatch();

  bool HasCaught();
  std::string GetStackTrace();

 private:
  v8::Isolate* isolate_;
  v8::TryCatch try_catch_;
};

}  // namespace nica

#endif  // CORE_TRY_CATCH_H_
