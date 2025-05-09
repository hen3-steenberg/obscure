module;
#include <array>
#include <type_traits>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
export module obscure.vulkan.pipeline:definition;
import :draw_call_base;
import :builder;

export namespace obscure::vulkan
{
        template<typename T>
        concept pipeline_definition = requires {
            typename T::shader_list;
            { T::shader_list::size() } -> std::convertible_to<std::size_t>;

            typename T::draw_calls;
            requires std::is_base_of_v<draw_call_base, typename T::draw_calls>;

        } &&
            requires (
                vk::Device d,
                vk::RenderPass r,
                std::array<vk::ShaderModule, T::shader_list::size()> s)
        {
            { T::initialize(d, r, s) } -> pipeline_builder;
        };

        template<pipeline_definition Pipeline>
        using builder_t = typename std::invoke_result_t<typename Pipeline::initialize, vk::Device, vk::RenderPass, std::array<vk::ShaderModule, Pipeline::shader_list::size()>>;
}