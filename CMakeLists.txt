# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)

if(DEFINED ENV{VCPKG_PATH})
	cmake_path(APPEND VCPKG_CMAKE "$ENV{VCPKG_PATH}" "scripts/buildsystems/vcpkg.cmake")
	include(${VCPKG_CMAKE})
endif()

project ("obscure")

include_directories("include")
add_subdirectory("src")
add_subdirectory("examples")