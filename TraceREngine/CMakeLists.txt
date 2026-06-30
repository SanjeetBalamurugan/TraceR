find_package(Vulkan REQUIRED)
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(TraceREngine SHARED ${ENGINE_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SOURCES})

target_include_directories(TraceREngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(TraceREngine PRIVATE ENGINE_EXPORT)

target_link_libraries(TraceREngine
    PRIVATE glfw
    PUBLIC glad
)

if(Vulkan_FOUND)
    target_link_libraries(TraceREngine PRIVATE Vulkan::Vulkan)
    target_compile_definitions(TraceREngine PUBLIC ENGINE_SUPPORT_VULKAN)
endif()

if(WIN32)
    target_link_libraries(TraceREngine PRIVATE d3d12 dxgi d3dcompiler)
    target_compile_definitions(TraceREngine PUBLIC ENGINE_SUPPORT_D3D12)
endif()

target_precompile_headers(TraceREngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h"
)