# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.28)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

if(DEFINED ENV{VCPKG_PATH})
	cmake_path(APPEND VCPKG_CMAKE "$ENV{VCPKG_PATH}" "scripts/buildsystems/vcpkg.cmake")
	include(${VCPKG_CMAKE})
endif()





if(MSVC)
	add_compile_options(/EHa)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
		add_compile_options(/EHa)
	else()
		add_compile_options(-fexceptions)
	endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-fexceptions)
endif()

project ("obscure" CXX)

add_subdirectory("resources")
# include_directories("include")
# add_subdirectory("src")
add_subdirectory("obscure")
add_subdirectory("examples")