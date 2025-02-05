#ifndef OBSCURE_VULKAN_DEVICE_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_DEVICE_CONTEXT_DEFINITION 1
#include "obscure/vulkan/device.hpp"
#include "obscure/vulkan/surface.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/pipeline_definition.hpp"
#include "obscure/vulkan/shader_set.hpp"
#include "obscure/vulkan/swap_chain.hpp"
#include "obscure/vulkan/pipeline_collection.hpp"
#include <functional>

namespace obscure::vulkan
{

    template<pipeline_definition ... TPipelines>
    struct device_context final : device, make_shader_set_t<TPipelines...>, swap_chain, pipeline_collection<sizeof...(TPipelines)>
    {
        using shader_set_t = make_shader_set_t<TPipelines...>;
        using pipeline_collection_t = pipeline_collection<sizeof...(TPipelines)>;

        device_context(surface const& _surface, glfw::glfw_window_ref _window, std::function<float (vk::PhysicalDevice)> get_device_score)
            : device(_surface, std::move(get_device_score)),
              make_shader_set_t<TPipelines...>(device::get_device()),
              swap_chain(*this, _surface, _window),
              pipeline_collection<sizeof...(TPipelines)>(pipeline_collection_t::template make_pipeline_collection<shader_set_t, TPipelines...>(device::get_device(), get_render_pass(), static_cast<shader_set_t const&>(*this)))
        {}

        [[nodiscard]] shader_set_t const& get_shader_set() const noexcept
        {
            return static_cast<shader_set_t const&>(*this);
        }
    };
}
#endif