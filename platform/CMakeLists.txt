find_library(COCOA_LIBRARY Cocoa)

list(APPEND NICA_PLATFROM_LIBRARY
    ${COCOA_LIBRARY}
)

set(NICA_PLATFROM_SOURCE 
    window/os_window.cc
    window/osx/osx_window.mm)

add_library(libnicaplatform ${NICA_PLATFROM_SOURCE})

target_link_libraries(libnicaplatform PUBLIC ${NICA_PLATFROM_LIBRARY})