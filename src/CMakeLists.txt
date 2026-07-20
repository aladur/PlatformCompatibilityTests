set(lib_SOURCES
    Utilities.cpp
    HostNameUtilities.cpp
)
set(lib_HEADER
    Utilities.h
    HostNameUtilities.h
)

add_library(LibPCT STATIC ${lib_SOURCES} ${lib_HEADRER})
add_library(PCT::LibPCT ALIAS LibPCT)

target_link_libraries(LibPCT PRIVATE
)

add_executable(PlatformCompatibilityTests
    main.cpp
    ${lib_HEADER}
)

target_link_libraries(PlatformCompatibilityTests PRIVATE
    PCT::LibPCT
)

