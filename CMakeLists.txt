# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)

if(DEFINED ENV{VCPKG_PATH})
	cmake_path(APPEND VCPKG_CMAKE "$ENV{VCPKG_PATH}" "scripts/buildsystems/vcpkg.cmake")
	include(${VCPKG_CMAKE})
endif()



project ("obscure" CXX)

if(MSVC)
	add_compile_options(/EHa)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
		add_compile_options(/EHa)
	else()
		add_compile_options(-fexceptions)
	endif()
else()
	add_compile_options(-fexceptions)
endif()

add_subdirectory("resources")
include_directories("include")
add_subdirectory("src")
add_subdirectory("examples")