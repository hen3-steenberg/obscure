# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEFINED ENV{VCPKG_PATH})
	if(NOT DEFINED VCPKG_TARGET_TRIPLET)
		if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
			set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}")
		elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			set(VCPKG_TARGET_TRIPLET "x64-linux")
		endif()
	endif()
	cmake_path(APPEND VCPKG_CMAKE "$ENV{VCPKG_PATH}" "scripts/buildsystems/vcpkg.cmake")
	include(${VCPKG_CMAKE})
endif()



project ("obscure" CXX)


find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

add_subdirectory("shaders")
add_subdirectory("utils")
add_subdirectory("helper_templates")
add_subdirectory("config")
add_subdirectory("glfw")
add_subdirectory("vulkan")
add_subdirectory("modules")
add_subdirectory("developer_test")
