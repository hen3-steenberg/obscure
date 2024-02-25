file(GLOB_RECURSE module_sources "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cppm")
file(GLOB_RECURSE module_definitions "${CMAKE_CURRENT_SOURCE_DIR}/import/*.cppm")

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

add_library(Obscure)

target_include_directories(Obscure PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(Obscure
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    ${module_definitions}
    ${module_sources}
)

target_link_libraries(Obscure PUBLIC glfw)
target_link_libraries(Obscure PUBLIC glm::glm)
target_link_libraries(Obscure PUBLIC Vulkan::Vulkan)
