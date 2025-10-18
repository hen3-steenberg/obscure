add_library(obscure_config)
add_library(obscure::config ALIAS obscure_config)
target_sources(obscure_config
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.properties.cppm"
)

target_link_libraries(obscure_config PUBLIC obscure::dependencies)
target_link_libraries(obscure_config PUBLIC obscure::utils)