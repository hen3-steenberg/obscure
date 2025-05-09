module;
#include <span>
#include <cstdint>
#include <type_traits>
export module obscure.vulkan.shader:data;

export namespace obscure::vulkan
{
        template<typename T>
        concept shader_data = std::is_convertible_v<T, std::span<const uint32_t>>;
}