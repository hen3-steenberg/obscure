add_library(obscure_builtin)
add_library(obscure::builtin ALIAS obscure_builtin)
target_sources(obscure_builtin
    PRIVATE
         "${CMAKE_CURRENT_SOURCE_DIR}/shader_bin/obscure.builtin.shaders.cxx"
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        "obscure.builtin.shaders.cppm"
        "obscure.builtin.pipelines.cppm"
        "obscure.builtin.pipelines.test_triangle.cppm"
        "obscure.builtin.pipelines.color_2d.cppm"
        "obscure.builtin.pipelines.texture_2d2d.cppm"
        "obscure.builtin.pipelines.texture_3d2d.cppm"
        "obscure.builtin.cppm"
)

target_link_libraries(obscure_builtin PUBLIC obscure::dependencies)
target_link_libraries(obscure_builtin PUBLIC obscure::utils)
target_link_libraries(obscure_builtin PUBLIC obscure::helper_templates)
target_link_libraries(obscure_builtin PUBLIC obscure::config)
target_link_libraries(obscure_builtin PUBLIC obscure::glfw)
target_link_libraries(obscure_builtin PUBLIC obscure::vulkan)
add_dependencies(obscure_builtin obscure_shaders)