#ifndef CORE_PER_ISOLATE_DATA_H_
#define CORE_PER_ISOLATE_DATA_H_

#include <map>
#include <memory>

#include "core/wrapper_info.h"
#include "v8-array-buffer.h"
#include "v8-forward.h"

namespace nica {

class IndexedPropertyInterceptor;
class NamedPropertyInterceptor;
class WrappableBase;

class PerIsolateData {
 public:
  PerIsolateData(v8::Isolate* isolate,
                 v8::ArrayBuffer::Allocator* allocator);
  PerIsolateData(const PerIsolateData&) = delete;
  PerIsolateData& operator=(const PerIsolateData&) = delete;
  ~PerIsolateData();

  static PerIsolateData* From(v8::Isolate* isolate);

  // Each isolate is associated with a collection of v8::ObjectTemplates and
  // v8::FunctionTemplates. Typically these template objects are created
  // lazily.
  void SetObjectTemplate(WrapperInfo* info,
                         v8::Local<v8::ObjectTemplate> object_template);
  void SetFunctionTemplate(WrapperInfo* info,
                           v8::Local<v8::FunctionTemplate> function_template);

  // These are low-level functions for retrieving object or function templates
  // stored in this object. Because these templates are often created lazily,
  // most clients should call higher-level functions that know how to populate
  // these templates if they haven't already been created.
  v8::Local<v8::ObjectTemplate> GetObjectTemplate(WrapperInfo* info);
  v8::Local<v8::FunctionTemplate> GetFunctionTemplate(WrapperInfo* info);

  // We maintain a map from Wrappable objects that derive from one of the
  // interceptor interfaces to the interceptor interface pointers.
  void SetIndexedPropertyInterceptor(WrappableBase* base,
                                     IndexedPropertyInterceptor* interceptor);
  void SetNamedPropertyInterceptor(WrappableBase* base,
                                   NamedPropertyInterceptor* interceptor);

  void ClearIndexedPropertyInterceptor(WrappableBase* base,
                                       IndexedPropertyInterceptor* interceptor);
  void ClearNamedPropertyInterceptor(WrappableBase* base,
                                     NamedPropertyInterceptor* interceptor);

  IndexedPropertyInterceptor* GetIndexedPropertyInterceptor(
      WrappableBase* base);
  NamedPropertyInterceptor* GetNamedPropertyInterceptor(WrappableBase* base);

  v8::Isolate* isolate() { return isolate_; }
  v8::ArrayBuffer::Allocator* allocator() { return allocator_; }

 private:
  typedef std::map<
      WrapperInfo*, v8::Eternal<v8::ObjectTemplate> > ObjectTemplateMap;
  typedef std::map<
      WrapperInfo*, v8::Eternal<v8::FunctionTemplate> > FunctionTemplateMap;
  typedef std::map<WrappableBase*, IndexedPropertyInterceptor*>
      IndexedPropertyInterceptorMap;
  typedef std::map<WrappableBase*, NamedPropertyInterceptor*>
      NamedPropertyInterceptorMap;

  // PerIsolateData doesn't actually own |isolate_|. Instead, the isolate is
  // owned by the JSIsolate, which also owns the PerIsolateData.
  v8::Isolate* isolate_;
  v8::ArrayBuffer::Allocator* allocator_;
  ObjectTemplateMap object_templates_;
  FunctionTemplateMap function_templates_;
  IndexedPropertyInterceptorMap indexed_interceptors_;
  NamedPropertyInterceptorMap named_interceptors_;
};

}  // namespace nica

#endif  // CORE_PER_ISOLATE_DATA_H_
