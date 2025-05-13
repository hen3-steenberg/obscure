module;
#include <functional>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.graphics_context;
export import obscure.glfw;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.surface;
export import obscure.vulkan.device;
export import obscure.vulkan.device_context;

export namespace obscure
{
    template<vulkan::pipeline_definition ... TPipelines>
    struct graphics_context
    {
#pragma region fields
        obscure::glfw::glfw_window window;
        obscure::vulkan::surface vk_surface;
        obscure::vulkan::device_context<TPipelines...> device_ctx;
        using command_session_t = obscure::vulkan::command_session<TPipelines...>;
#pragma endregion

#pragma region user_functions
        inline command_session_t begin_frame()
        {
            return device_ctx.begin_frame();
        }

        inline void submit_frame()
        {
            device_ctx.submit_frame();
        }

        inline void draw_frame()
        {
            device_ctx.draw_frame();
        }

#pragma endregion


        graphics_context(std::function<float (vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window(), vk_surface(window), device_ctx(vk_surface, window, get_device_score)
        {
        }
    };
}