# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)

if(DEFINED ENV{VCPKG_PATH})
	cmake_path(APPEND VCPKG_CMAKE "$ENV{VCPKG_PATH}" "scripts/buildsystems/vcpkg.cmake")
	include(${VCPKG_CMAKE})
endif()



project ("obscure" CXX)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

add_subdirectory("obscure")
add_subdirectory("developer_test")
add_subdirectory("examples")