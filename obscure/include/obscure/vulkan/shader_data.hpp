#ifndef OBSCURE_VULKAN_SHADER_DATA_DEFINITION
#define OBSCURE_VULKAN_SHADER_DATA_DEFINITION 1
#include <span>
#include <filesystem>
#include <fstream>
#include <vector>

template<auto shader_key>
std::span<const uint32_t> get_shader_data() = delete;

#endif