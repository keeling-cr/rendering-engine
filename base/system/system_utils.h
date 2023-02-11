#ifndef BASE_SYSTEM_SYSTEM_UTILS_H_
#define BASE_SYSTEM_SYSTEM_UTILS_H_

#include <functional>
#include <string>

namespace base {

void* OpenSystemLibrary(const char* full_path);
void* OpenSystemLibraryAndGetError(const char* full_path, std::string* error_out);

void* GetLibrarySymbol(void* library_handle, const char*  symbol_name);
void CloseSystemLibrary(void* library_handle);

class Library {
  public:
    Library() {}
    Library(void* library_handle) : library_handle_(library_handle) {}
    ~Library() { Close(); }

    bool Open(const char* full_path) {
        Close();
        library_handle_ = OpenSystemLibrary(full_path);
        return library_handle_ != nullptr;
    }

    bool OpenAndGetError(const char* full_path, std::string* error_out) {
        Close();
        library_handle_ = OpenSystemLibraryAndGetError(full_path, error_out);
        return library_handle_ != nullptr;
    }

    void Close() {
        if (library_handle_) {
            CloseSystemLibrary(library_handle_);
            library_handle_ = nullptr;
        }
    }

    void *GetSymbol(const char* symbol_name) { return GetLibrarySymbol(library_handle_, symbol_name); }

    void *GetNative() const { return library_handle_; }

    template <typename FuncT>
    void GetAs(const char *symbolName, FuncT *funcOut) {
        *funcOut = reinterpret_cast<FuncT>(GetSymbol(symbolName));
    }

  private:
    void* library_handle_ = nullptr;
};

Library *OpenSharedLibrary(const char* full_path);
Library *OpenSharedLibraryAndGetError(const char* full_path);

}  // namespace base

#endif  // BASE_SYSTEM_SYSTEM_UTILS_H_
