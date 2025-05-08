add_library(obscure_vulkan)
target_sources(obscure_vulkan
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "application_context.cppm"
        "device.cppm"
        "graphics_context.cppm"
        "logger.cppm"
        "pipeline.cppm"
        "pipeline-builder.cppm"
        "pipeline-collection.cppm"
        "pipeline-definition.cppm"
        "pipeline-draw_call_base.cppm"
        "surface.cppm"
)

target_link_libraries(obscure_vulkan PUBLIC glfw)
target_link_libraries(obscure_vulkan PUBLIC glm::glm)
target_link_libraries(obscure_vulkan PUBLIC Vulkan::Vulkan)

target_link_libraries(obscure_vulkan PUBLIC utils)
target_link_libraries(obscure_vulkan PUBLIC helper_templates)
target_link_libraries(obscure_vulkan PUBLIC config)
target_link_libraries(obscure_vulkan PUBLIC obscure_glfw)
add_dependencies(obscure_vulkan shaders)