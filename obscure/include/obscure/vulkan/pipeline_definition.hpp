//
// Created by hen3 on 2024/12/24.
//

#ifndef OBSCURE_VULKAN_PIPELINE_DEFINITION_HPP
#define OBSCURE_VULKAN_PIPELINE_DEFINITION_HPP
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/pipeline_builder.hpp"
#include "obscure/vulkan/draw_call_base.hpp"
#include <array>
#include <type_traits>

namespace obscure {
    namespace vulkan {
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
}

#endif //PIPELINE_DEFINITION_HPP
