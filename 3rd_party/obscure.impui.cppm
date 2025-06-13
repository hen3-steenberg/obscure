module;
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#include <vulkan/vulkan.hpp>
export module obscure.imgui;
import obscure.vulkan.application_context;

export namespace obscure::imgui
{
    struct ctx
    {
        ImGuiContext * data;
    private:
        static ImGuiContext * create_ctx()
        {
            IMGUI_CHECKVERSION();
            return ImGui::CreateContext();
        }
    public:
        template<typename GfxCtxT>
        explicit ctx(GfxCtxT const& gfx_ctx)
            : data(create_ctx())
        {
            ImGui_ImplGlfw_InitForVulkan(gfx_ctx.window.window_ptr, true);
            ImGui_ImplVulkan_InitInfo imgui_init_info{
                .Instance = obscure::get_application_instance(),
                .PhysicalDevice = gfx_ctx.device_ctx.vk_device.get_physical_device(),
                .Device = gfx_ctx.device_ctx.vk_device.get_device(),
                .QueueFamily = gfx_ctx.device_ctx.vk_device.get_graphics_queue_index(),
                .Queue = gfx_ctx.device_ctx.vk_device.get_graphics_queue(),
                .Subpass = 0,
                .MinImageCount = gfx_ctx.device_ctx.vk_swap_chain.image_count,
                .ImageCount = gfx_ctx.device_ctx.vk_swap_chain.image_count,
                .DescriptorPoolSize = gfx_ctx.device_ctx.vk_swap_chain.image_count * 2,
                .MSAASamples = static_cast<VkSampleCountFlagBits>(gfx_ctx.device_ctx.vk_swap_chain.multisaa_buffer.msaa_samples),
                .RenderPass = gfx_ctx.device_ctx.vk_swap_chain.render_pass
            };
            ImGui_ImplVulkan_Init(&imgui_init_info);
        }

        ctx(ctx const&) = delete;
        ctx(ctx && other) noexcept
            : data(other.data)
        {
            other.data = nullptr;
        }

        ImGuiContext * operator->() noexcept
        {
            return data;
        }

        const ImGuiContext * operator->() const noexcept
        {
            return data;
        }

        explicit operator ImGuiContext*() const noexcept
        {
            return data;
        }

        ~ctx()
        {
            if (data)
            {
                ImGui_ImplVulkan_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext(data);
            }
        }
    };

    struct frame
    {
        vk::CommandBuffer command_buffer;
        frame(vk::CommandBuffer command_buffer_)
            : command_buffer(command_buffer_)
        {
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        frame(frame const&) = delete;
        frame(frame && other) noexcept
            : command_buffer(other.command_buffer)
        {
            other.command_buffer = VK_NULL_HANDLE;
        }

        ~frame()
        {
            if (command_buffer != VK_NULL_HANDLE)
            {
                ImGui::Render();
                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), command_buffer);
            }
        }
    };
}