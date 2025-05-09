add_library(config)
target_sources(config
        PUBLIC
        FILE_SET CXX_MODULES FILES
        "obscure.properties.cppm"
)

target_link_libraries(config PUBLIC Vulkan::Vulkan)
target_link_libraries(config PUBLIC utils)