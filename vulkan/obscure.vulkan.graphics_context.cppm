module;
#include "stb/stb_image.h"
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <span>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.graphics_context;
export import obscure.glfw;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.surface;
export import obscure.vulkan.device;
export import obscure.vulkan.device_context;

export namespace obscure
{
    template<vulkan::pipeline_definition... TPipelines>
    struct graphics_context {
#pragma region fields
        obscure::glfw::glfw_window window;
        obscure::vulkan::surface vk_surface;
        obscure::vulkan::device_context<TPipelines...> device_ctx;
        using command_session_t = obscure::vulkan::command_session<TPipelines...>;
#pragma endregion

#pragma region user_functions
        inline command_session_t
        begin_frame()
        {
            return device_ctx.begin_frame();
        }

        inline void
        submit_frame()
        {
            device_ctx.submit_frame();
        }

        inline void
        draw_frame()
        {
            device_ctx.draw_frame();
        }

        template<typename T>
        [[nodiscard]] auto
        init_vertex_buffer(std::initializer_list<T> data) const
        {
            return device_ctx.template initialize_vertex_buffer<T>(std::span<const T>{ data.begin(), data.size() });
        }

        template<typename T>
        [[nodiscard]] auto
        init_vertex_buffer(std::span<const T> data) const
        {
            return device_ctx.template initialize_vertex_buffer<T>(data);
        }

        template<typename T>
        void
        reinitialize_vertex_buffer(obscure::vulkan::staging_buffer<T> const& newdata,
                                   std::size_t count,
                                   obscure::vulkan::vertex_buffer<T>& vertex_buffer) const
        {
            device_ctx.template reinitialize_vertex_buffer<T>(newdata, count, vertex_buffer);
        }

        template<obscure::vulkan::vk_index T>
        [[nodiscard]] auto
        init_index_buffer(std::initializer_list<T> data) const
        {
            return device_ctx.template initialize_index_buffer<T>(std::span<const T>{ data.begin(), data.size() });
        }

        template<obscure::vulkan::vk_index T>
        [[nodiscard]] auto
        init_index_buffer(std::span<const T> data) const
        {
            return device_ctx.template initialize_index_buffer<T>(data);
        }

        template<typename T, obscure::vulkan::pipeline_definition TPipeline>
        [[nodiscard]] obscure::vulkan::uniform_buffer<T>
        create_uniform(uint32_t binding) const
        {
            return device_ctx.template create_uniform<T, TPipeline>(binding);
        }

        template<obscure::vulkan::pipeline_definition TPipeline,
                 vk::SamplerAddressMode modeU = vk::SamplerAddressMode::eRepeat,
                 vk::SamplerAddressMode modeV = modeU,
                 vk::SamplerAddressMode modeW = modeV>
        [[nodiscard]] obscure::vulkan::rgba_2d_texture<modeU, modeV, modeW>
        load_texture(std::filesystem::path const& image_path, uint32_t binding) const
        {
            if (!std::filesystem::exists(image_path)) {
                throw std::runtime_error("Failed to load texture: " + image_path.string() +
                                         " cwd: " + std::filesystem::current_path().string());
            }
            return device_ctx.template load_texture<TPipeline, modeU, modeV, modeW>(image_path, binding);
        }

        template<obscure::vulkan::pipeline_definition TPipeline,
                 vk::SamplerAddressMode modeU = vk::SamplerAddressMode::eRepeat,
                 vk::SamplerAddressMode modeV = modeU,
                 vk::SamplerAddressMode modeW = modeV>
        [[nodiscard]] obscure::vulkan::rgba_2d_texture<modeU, modeV, modeW>
        load_texture(std::span<const stbi_uc> data, uint32_t binding) const
        {
            return device_ctx.template load_texture<TPipeline, modeU, modeV, modeW>(data, binding);
        }

#pragma endregion

        explicit graphics_context(
            std::function<float(vk::PhysicalDevice)> get_device_score = vulkan::get_device_score_default)
            : window()
            , vk_surface(window)
            , device_ctx(vk_surface, window, get_device_score)
        {
        }
    };
} // namespace obscure
