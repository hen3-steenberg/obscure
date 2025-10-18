add_library(obscure_glfw)
add_library(obscure::glfw ALIAS obscure_glfw)
target_sources(obscure_glfw
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.glfw.cppm"
        "obscure.glfw-defaults.cppm"
        "obscure.glfw-window.cppm"
)

target_link_libraries(obscure_glfw PUBLIC obscure::dependencies)
target_link_libraries(obscure_glfw PUBLIC obscure::config)