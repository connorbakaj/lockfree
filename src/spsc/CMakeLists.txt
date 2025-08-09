add_library(lockfree_spsc_queue INTERFACE)

# Public include directory points to this src folder (or adjust if you use include/)
target_include_directories(lockfree_spsc_queue
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler flags for performance and warnings
if(MSVC)
    target_compile_options(lockfree_spsc_queue INTERFACE /O2 /GL /arch:AVX2 /W4)
else()
    target_compile_options(lockfree_spsc_queue INTERFACE
        -O3
        -march=native
        -mtune=native
        -flto
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
