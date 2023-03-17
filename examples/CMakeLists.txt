find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

add_executable(TRIANGLES triangles.cpp ${OBSCURE_SOURCE})

target_link_libraries(TRIANGLES PRIVATE glfw)
target_link_libraries(TRIANGLES PRIVATE glm::glm)
target_link_libraries(TRIANGLES PRIVATE Vulkan::Vulkan)