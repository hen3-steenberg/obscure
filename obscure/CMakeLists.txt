add_library(obscure)
target_sources(obscure
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.cppm"
)

target_link_libraries(obscure PUBLIC glfw)
target_link_libraries(obscure PUBLIC glm::glm)
target_link_libraries(obscure PUBLIC Vulkan::Vulkan)
target_link_libraries(obscure PUBLIC GPUOpen::VulkanMemoryAllocator)

target_link_libraries(obscure PUBLIC utils)
target_link_libraries(obscure PUBLIC helper_templates)
target_link_libraries(obscure PUBLIC config)
target_link_libraries(obscure PUBLIC obscure_glfw)
target_link_libraries(obscure PUBLIC obscure_vulkan)
target_link_libraries(obscure PUBLIC builtin)

