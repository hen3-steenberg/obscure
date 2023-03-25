file(GLOB_RECURSE src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

add_library(Obscure STATIC ${src})

target_link_libraries(Obscure PRIVATE glfw)
target_link_libraries(Obscure PRIVATE glm::glm)
target_link_libraries(Obscure PRIVATE Vulkan::Vulkan)