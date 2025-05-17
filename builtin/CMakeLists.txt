add_library(builtin)
target_sources(builtin
    PRIVATE
         "${CMAKE_CURRENT_SOURCE_DIR}/shader_bin/obscure.builtin.shaders.cxx"
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        "obscure.builtin.shaders.cppm"
        "obscure.builtin.pipelines.cppm"
        "obscure.builtin.pipelines-test_triangle.cppm"
        "obscure.builtin.pipelines-color_2d.cppm"
        "obscure.builtin.cppm"
)

target_link_libraries(builtin PUBLIC glfw)
target_link_libraries(builtin PUBLIC glm::glm)
target_link_libraries(builtin PUBLIC Vulkan::Vulkan)
target_link_libraries(builtin PUBLIC GPUOpen::VulkanMemoryAllocator)

target_link_libraries(builtin PUBLIC utils)
target_link_libraries(builtin PUBLIC helper_templates)
target_link_libraries(builtin PUBLIC config)
target_link_libraries(builtin PUBLIC obscure_glfw)
target_link_libraries(builtin PUBLIC obscure_vulkan)
add_dependencies(builtin shaders)