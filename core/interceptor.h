#ifndef CORE_INTERCEPTOR_H_
#define CORE_INTERCEPTOR_H_

#include <stdint.h>

#include <string>
#include <vector>

#include "v8-forward.h"

namespace nica {

class WrappableBase;

class NamedPropertyInterceptor {
 public:
  NamedPropertyInterceptor(v8::Isolate* isolate, WrappableBase* base);
  NamedPropertyInterceptor(const NamedPropertyInterceptor&) = delete;
  NamedPropertyInterceptor& operator=(const NamedPropertyInterceptor&) = delete;
  virtual ~NamedPropertyInterceptor();

  virtual v8::Local<v8::Value> GetNamedProperty(v8::Isolate* isolate,
                                                const std::string& property);
  virtual bool SetNamedProperty(v8::Isolate* isolate,
                                const std::string& property,
                                v8::Local<v8::Value> value);
  virtual std::vector<std::string> EnumerateNamedProperties(
      v8::Isolate* isolate);

 private:
  v8::Isolate* isolate_;
  WrappableBase* base_;
};

class IndexedPropertyInterceptor {
 public:
  IndexedPropertyInterceptor(v8::Isolate* isolate, WrappableBase* base);
  IndexedPropertyInterceptor(const IndexedPropertyInterceptor&) = delete;
  IndexedPropertyInterceptor& operator=(const IndexedPropertyInterceptor&) =
      delete;
  virtual ~IndexedPropertyInterceptor();

  virtual v8::Local<v8::Value> GetIndexedProperty(v8::Isolate* isolate,
                                                  uint32_t index);
  virtual bool SetIndexedProperty(v8::Isolate* isolate,
                                  uint32_t index,
                                  v8::Local<v8::Value> value);
  virtual std::vector<uint32_t> EnumerateIndexedProperties(
      v8::Isolate* isolate);

 private:
  v8::Isolate* isolate_;
  WrappableBase* base_;
};

}  // namespace nica 

#endif  // CORE_INTERCEPTOR_H_
