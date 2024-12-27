//
// Created by hen3 on 2024/12/24.
//

#ifndef OBSCURE_VULKAN_PIPELINE_DEFINITION_HPP
#define OBSCURE_VULKAN_PIPELINE_DEFINITION_HPP
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/pipeline_builder.hpp"
#include <array>
#include <type_traits>

namespace obscure {
    namespace vulkan {
        template<typename T>
        concept pipeline_definition = requires {
            typename T::shader_list;
            typename T::pipeline;
            { T::shader_list::size() } -> std::convertible_to<std::size_t>;
        } && requires (vk::Device d, vk::RenderPass r, std::array<vk::ShaderModule, T::shader_list::size()> s)
        {
            { T::initialize(d, r, s) } -> pipeline_builder;
        };
    }
}

#endif //PIPELINE_DEFINITION_HPP
