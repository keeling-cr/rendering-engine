#include "base/system/system_utils.h"

#include <stdlib.h>

namespace base {

Library* OpenSharedLibrary(const char *full_path) {
    void *libraryHandle = OpenSystemLibraryAndGetError(full_path, nullptr);
    return new Library(libraryHandle);
}

void* OpenSystemLibrary(const char *full_path) {
    return OpenSystemLibraryAndGetError(full_path, nullptr);
}

}  // namespace base
