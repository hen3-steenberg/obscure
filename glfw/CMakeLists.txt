add_library(obscure_glfw)
target_sources(obscure_glfw
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.glfw.cppm"
        "obscure.glfw-defaults.cppm"
        "obscure.glfw-window.cppm"
)

target_link_libraries(obscure_glfw PUBLIC glfw)
target_link_libraries(obscure_glfw PUBLIC Vulkan::Vulkan)
target_link_libraries(obscure_glfw PUBLIC config)