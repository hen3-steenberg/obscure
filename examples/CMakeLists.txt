find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

add_executable(TRIANGLES triangles.cpp)

add_dependencies(TRIANGLES OBSCURE_SHADERS)

add_custom_command(TARGET TRIANGLES POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:TRIANGLES>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_BINARY_DIR}/shaders"
        "$<TARGET_FILE_DIR:TRIANGLES>/shaders"
        )

target_link_libraries(TRIANGLES PRIVATE Obscure)
target_link_libraries(TRIANGLES PRIVATE glfw)
target_link_libraries(TRIANGLES PRIVATE glm::glm)
target_link_libraries(TRIANGLES PRIVATE Vulkan::Vulkan)