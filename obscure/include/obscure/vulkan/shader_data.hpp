#ifndef OBSCURE_VULKAN_SHADER_DATA_DEFINITION
#define OBSCURE_VULKAN_SHADER_DATA_DEFINITION 1
#include <span>
#include <cstdint>
#include <type_traits>

namespace obscure {
    namespace vulkan {
        template<typename T>
        concept shader_data = std::is_convertible_v<T, std::span<const uint32_t>>;
    }
}

#endif