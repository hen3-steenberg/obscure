add_library(obscure_deps STATIC)
add_library(obscure::dependencies ALIAS obscure_deps)
target_include_directories(obscure_deps PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(obscure_deps
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/vma.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp"
)

target_link_libraries(obscure_deps PUBLIC glfw)
target_link_libraries(obscure_deps PUBLIC glm::glm)
target_link_libraries(obscure_deps PUBLIC Vulkan::Vulkan)
target_link_libraries(obscure_deps PUBLIC GPUOpen::VulkanMemoryAllocator)