add_executable(OBSCURE_DEV_APP "testapp.cpp")

target_link_libraries(OBSCURE_DEV_APP PRIVATE obscure)
target_link_libraries(OBSCURE_DEV_APP PRIVATE glfw)
target_link_libraries(OBSCURE_DEV_APP PRIVATE glm::glm)
target_link_libraries(OBSCURE_DEV_APP PRIVATE Vulkan::Vulkan)
