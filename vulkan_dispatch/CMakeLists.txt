add_library(obscure_vulkan_dispatch)
add_library(obscure::vulkan_dispatch ALIAS obscure_vulkan_dispatch)
target_sources(obscure_vulkan_dispatch
    PRIVATE
        "obscure.vulkan.dispatch.instance.cxx"
    PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.vulkan.dispatch.instance.cppm"
        "obscure.vulkan.result.cppm"
)

target_link_libraries(obscure_vulkan_dispatch PUBLIC obscure::dependencies)
target_link_libraries(obscure_vulkan_dispatch PUBLIC obscure::utils)
target_link_libraries(obscure_vulkan_dispatch PUBLIC obscure::helper_templates)
