add_library(obscure_vulkan)
add_library(obscure::vulkan ALIAS obscure_vulkan)
target_sources(obscure_vulkan
        PRIVATE
        "obscure.vulkan.application_context.cxx"
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.vulkan.application_context.cppm"
        "obscure.vulkan.buffer.cppm"
        "obscure.vulkan.command_pool.cppm"
        "obscure.vulkan.command_session.cppm"
        "obscure.vulkan.depth_buffer.cppm"
        "obscure.vulkan.device.cppm"
        "obscure.vulkan.device_context.cppm"
        "obscure.vulkan.fence.cppm"
        "obscure.vulkan.graphics_context.cppm"
        "obscure.vulkan.image_view.cppm"
        "obscure.vulkan.msaa_buffer.cppm"
        "obscure.vulkan.pipeline.cppm"
        "obscure.vulkan.pipeline-builder.cppm"
        "obscure.vulkan.pipeline-collection.cppm"
        "obscure.vulkan.pipeline-definition.cppm"
        "obscure.vulkan.pipeline-draw_call_base.cppm"
        "obscure.vulkan.transfer_session.cppm"
        "obscure.vulkan.semaphore.cppm"
        "obscure.vulkan.shader.cppm"
        "obscure.vulkan.shader-data.cppm"
        "obscure.vulkan.shader-modules.cppm"
        "obscure.vulkan.shader-set.cppm"
        "obscure.vulkan.surface.cppm"
        "obscure.vulkan.swap_chain.cppm"
        "obscure.vulkan.texture.cppm"
        "obscure.vulkan.cppm"
)

target_link_libraries(obscure_vulkan PUBLIC obscure::dependencies)
target_link_libraries(obscure_vulkan PUBLIC obscure::utils)
target_link_libraries(obscure_vulkan PUBLIC obscure::helper_templates)
target_link_libraries(obscure_vulkan PUBLIC obscure::config)
target_link_libraries(obscure_vulkan PUBLIC obscure::glfw)