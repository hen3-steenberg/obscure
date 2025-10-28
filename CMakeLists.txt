# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wunused-variable -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEFINED ENV{VCPKG_ROOT})
	if(NOT DEFINED VCPKG_TARGET_TRIPLET)
		if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
			set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}")
		elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			set(VCPKG_TARGET_TRIPLET "x64-linux")
		endif()
	endif()
	cmake_path(APPEND VCPKG_CMAKE "$ENV{VCPKG_ROOT}" "scripts/buildsystems/vcpkg.cmake")
	include(${VCPKG_CMAKE})
endif()



project ("obscure" CXX)

if(NOT TARGET glfw)
	message(STATUS "Searching for GLFW.")
	find_package(glfw3 CONFIG REQUIRED)
endif()

if(NOT TARGET glm::glm)
	message(STATUS "Searching for glm.")
	find_package(glm CONFIG REQUIRED)
endif()

find_package(Vulkan)

if(NOT TARGET GPUOpen::VulkanMemoryAllocator)
	message(STATUS "Searching for VulkanMemoryAllocator.")
	find_package(VulkanMemoryAllocator CONFIG REQUIRED)
endif()

add_subdirectory("dependencies")
add_subdirectory("shaders")
add_subdirectory("utils")
add_subdirectory("helper_templates")
add_subdirectory("config")
add_subdirectory("glfw")
add_subdirectory("vulkan")
add_subdirectory("builtin")
add_subdirectory("obscure")
add_subdirectory("3rd_party")

if(NOT DEFINED OBSCURE_TEST OR OBSCURE_TEST)
	add_subdirectory("developer_test")
endif()

find_program(CLANG_FORMAT_EXE clang-format)
file(GLOB_RECURSE CXX_FILES "*.cppm" "*.cxx" "*.cpp" "*.hpp")
list(FILTER CXX_FILES EXCLUDE REGEX "3rd_party/imgui")
file(GLOB_RECURSE SHADER_FILES "*.vert" "*.frag")
list(FILTER SHADER_FILES EXCLUDE REGEX "3rd_party/imgui")

if(CLANG_FORMAT_EXE)
	add_custom_target(FORMAT_OBSCURE
			COMMENT "Formatting files with clang-format..."
			COMMAND ${CLANG_FORMAT_EXE} -i ${CXX_FILES} ${SHADER_FILES}
	)
	message(STATUS "Added 'format' target")
else()
	message(WARNING
			"clang-format not found - format target unavailable")
endif()
