add_library(obscure)
add_library(obscure::obscure ALIAS obscure)
target_sources(obscure
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.cppm"
)

target_link_libraries(obscure PUBLIC obscure::dependencies)
target_link_libraries(obscure PUBLIC obscure::utils)
target_link_libraries(obscure PUBLIC obscure::helper_templates)
target_link_libraries(obscure PUBLIC obscure::config)
target_link_libraries(obscure PUBLIC obscure::glfw)
target_link_libraries(obscure PUBLIC obscure::vulkan)
target_link_libraries(obscure PUBLIC obscure::builtin)

