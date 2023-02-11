#include "system_utils.h"


#include <dlfcn.h>
#include <string.h>

namespace base {

namespace {

void *OpenPosixLibrary(const std::string &fullPath, std::string *errorOut) {
    void *module = dlopen(fullPath.c_str(), RTLD_NOW);
    if (module) {
        if (errorOut) {
            *errorOut = fullPath;
        }
    } else if (errorOut) {
        *errorOut = "dlopen(";
        *errorOut += fullPath;
        *errorOut += ") failed with error: ";
        *errorOut += dlerror();
    }
    return module;
}

}  // namespace


void *OpenSystemLibraryAndGetError(const char *full_path, std::string* error_out) {
    return OpenPosixLibrary(full_path, error_out);
}

void *GetLibrarySymbol(void *libraryHandle, const char *symbolName) {
    if (!libraryHandle) {
        return nullptr;
    }

    return dlsym(libraryHandle, symbolName);
}

void CloseSystemLibrary(void *libraryHandle) {
    if (libraryHandle) {
        dlclose(libraryHandle);
    }
}

}  // namespace angle
