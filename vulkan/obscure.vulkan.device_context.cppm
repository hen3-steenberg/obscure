module;
#include "stb/stb_image.h"
#include <cmath>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <span>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.device_context;

export import obscure.vulkan.command_pool;
export import obscure.vulkan.command_session;
export import obscure.vulkan.device;
export import obscure.vulkan.pipeline;
export import obscure.vulkan.shader;
export import obscure.vulkan.surface;
export import obscure.vulkan.transfer_session;
export import obscure.vulkan.buffer;
export import obscure.vulkan.texture;
import obscure.helper_templates.index_of;

export namespace obscure::vulkan
{

    template<pipeline_definition... TPipelines>
    struct device_context final {
        using shader_set_t = make_shader_set_t<TPipelines...>;
        using pipeline_collection_t = pipeline_collection<sizeof...(TPipelines)>;
        using command_session_t = obscure::vulkan::command_session<TPipelines...>;

        device vk_device;
        shader_set_t vk_shaders;
        swap_chain vk_swap_chain;
        pipeline_collection_t vk_pipeline_collection;
        command_pool vk_command_pool;
        transfer_pool vk_transfer_pool;

        device_context(surface const& _surface,
                       glfw::glfw_window_ref _window,
                       std::function<float(vk::PhysicalDevice)> get_device_score)
            : vk_device(_surface, std::move(get_device_score))
            , vk_shaders(vk_device.get_device())
            , vk_swap_chain(vk_device, _surface, _window)
            , vk_pipeline_collection(
                  pipeline_collection_t::template make_pipeline_collection<shader_set_t, TPipelines...>(
                      vk_device.get_device(),
                      vk_swap_chain.get_render_pass(),
                      vk_swap_chain.multisaa_buffer.msaa_samples,
                      vk_shaders))
            , vk_command_pool(vk_device, vk_swap_chain.get_frame_count())
            , vk_transfer_pool(vk_device)
        {
        }

        ~device_context() noexcept { vk_device.waitIdle(); }

        [[nodiscard]] vk::CommandBuffer
        get_current_buffer() const noexcept
        {
            return vk_command_pool[vk_swap_chain.get_current_frame()];
        }

        [[nodiscard]] vk::CommandBuffer const*
        get_current_buffer_ptr() const noexcept
        {
            return &vk_command_pool[vk_swap_chain.get_current_frame()];
        }

        command_session_t
        begin_frame()
        {
            uint32_t frame_index = vk_swap_chain.get_next_frame_index();
            return command_session_t{ get_current_buffer(), frame_index, vk_swap_chain, vk_pipeline_collection };
        }

        transfer_session
        begin_transfers() const
        {
            return transfer_session{ vk_device, vk_transfer_pool.get_command_pool(), get_current_buffer() };
        }

        void
        submit_frame()
        {
            constexpr static const vk::PipelineStageFlags wait_stages[] = {
                vk::PipelineStageFlagBits::eColorAttachmentOutput
            };

            vk::SubmitInfo submit_info{
                1, vk_swap_chain.get_image_available_ptr(), wait_stages, 1, get_current_buffer_ptr(),
                1, vk_swap_chain.get_render_finished_ptr()
            };

            auto _ = vk_device.get_graphics_queue().submit(1, &submit_info, vk_swap_chain.get_in_flight_fence());
        }

        void
        draw_frame()
        {
            vk::PresentInfoKHR present_info{ 1,
                                             vk_swap_chain.get_render_finished_ptr(),
                                             1,
                                             vk_swap_chain.get_swap_chain_ptr(),
                                             vk_swap_chain.get_frame_index_ptr() };

            auto _ = vk_device.get_present_queue().presentKHR(present_info);
        }

        template<typename T>
        [[nodiscard]] obscure::vulkan::vertex_buffer<T>
        initialize_vertex_buffer(std::span<const T> data) const
        {
            obscure::vulkan::staging_buffer<T> temp_buffer{ vk_device, data.size() };
            temp_buffer.copy_data(data.data(), data.size() * sizeof(T));

            obscure::vulkan::vertex_buffer<T> vertex_buffer{ vk_device, data.size() };
            {
                transfer_session copy_session = begin_transfers();
                copy_session.transfer_data(temp_buffer, vertex_buffer);
            }

            return vertex_buffer;
        }

        template<typename T>
        void reinitialize_vertex_buffer(staging_buffer<T> const& newdata, std::size_t count, vertex_buffer<T> & vertex_buffer) const {
            if(count > vertex_buffer.count()) {
                vertex_buffer = obscure::vulkan::vertex_buffer<T>{ vk_device, newdata.count() };
            }

            vk_device.waitIdle();

            {
                transfer_session copy_session = begin_transfers();
                copy_session.transfer_data(newdata, vertex_buffer, count);
            }
        }

        template<vk_index T>
        [[nodiscard]] obscure::vulkan::index_buffer<T>
        initialize_index_buffer(std::span<const T> data) const
        {
            obscure::vulkan::staging_buffer<T> temp_buffer{ vk_device, data.size() };
            temp_buffer.copy_data(data.data(), data.size() * sizeof(T));

            obscure::vulkan::index_buffer<T> vertex_buffer{ vk_device, data.size() };
            {
                transfer_session copy_session = begin_transfers();
                copy_session.transfer_data(temp_buffer, vertex_buffer);
            }

            return vertex_buffer;
        }

        template<typename T, obscure::vulkan::pipeline_definition TPipeline>
        [[nodiscard]] obscure::vulkan::uniform_buffer<T>
        create_uniform(uint32_t binding) const
        {
            constexpr size_t pipeline_idx = obscure::helper_templates::index_of<TPipeline, TPipelines...>();
            return obscure::vulkan::uniform_buffer<T>{ vk_device,
                                                       vk_pipeline_collection.uniform_descriptor_layouts[pipeline_idx],
                                                       binding };
        }

        template<obscure::vulkan::pipeline_definition TPipeline,
                 vk::SamplerAddressMode modeU,
                 vk::SamplerAddressMode modeV,
                 vk::SamplerAddressMode modeW>
        [[nodiscard]] obscure::vulkan::rgba_2d_texture<modeU, modeV, modeW>
        load_texture(std::filesystem::path const& image_path, uint32_t binding) const
        {
            constexpr size_t pipeline_idx = obscure::helper_templates::index_of<TPipeline, TPipelines...>();
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(image_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            size_t imageSize = texWidth * texHeight * 4;
            uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
            vk::Extent3D image_extent{ static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1 };

            obscure::vulkan::staging_buffer<stbi_uc> temp_buffer{ vk_device, imageSize };
            temp_buffer.copy_data(pixels, imageSize);

            obscure::vulkan::rgba_2d_texture result{ vk_device,
                                                     image_extent,
                                                     vk_pipeline_collection.texture_descriptor_layouts[pipeline_idx],
                                                     binding,
                                                     mipLevels };

            {
                transfer_session copy_session = begin_transfers();
                copy_session.copy_buffer_to_texture(temp_buffer, result);
            }
            return result;
        }

        template<obscure::vulkan::pipeline_definition TPipeline,
                 vk::SamplerAddressMode modeU,
                 vk::SamplerAddressMode modeV,
                 vk::SamplerAddressMode modeW>
        [[nodiscard]] obscure::vulkan::rgba_2d_texture<modeU, modeV, modeW>
        load_texture(std::span<const stbi_uc> data, uint32_t binding) const
        {
            constexpr size_t pipeline_idx = obscure::helper_templates::index_of<TPipeline, TPipelines...>();
            int texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load_from_memory(
                data.data(), static_cast<int>(data.size()), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            size_t imageSize = texWidth * texHeight * 4;
            uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
            vk::Extent3D image_extent{ static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1 };

            obscure::vulkan::staging_buffer<stbi_uc> temp_buffer{ vk_device, imageSize };
            temp_buffer.copy_data(pixels, imageSize);

            obscure::vulkan::rgba_2d_texture result{ vk_device,
                                                     image_extent,
                                                     vk_pipeline_collection.texture_descriptor_layouts[pipeline_idx],
                                                     binding,
                                                     mipLevels };

            {
                transfer_session copy_session = begin_transfers();
                copy_session.copy_buffer_to_texture(temp_buffer, result);
            }
            return result;
        }
    };
} // namespace obscure::vulkan
