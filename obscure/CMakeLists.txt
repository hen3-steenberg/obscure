add_subdirectory("resources")

file(GLOB_RECURSE src "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(Obscure STATIC ${src})

target_include_directories(Obscure PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(Obscure PUBLIC glfw)
target_link_libraries(Obscure PUBLIC glm::glm)
target_link_libraries(Obscure PUBLIC Vulkan::Vulkan)
add_dependencies(Obscure OBSCURE_SHADERS)
