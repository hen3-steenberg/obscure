module;
#include <array>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.swap_chain;

export import obscure.vulkan.device;
export import obscure.vulkan.fence;
export import obscure.glfw;
export import obscure.vulkan.image_view;
import obscure.properties;
export import obscure.vulkan.semaphore;
export import obscure.vulkan.depth_buffer;
export import obscure.vulkan.msaa_buffer;

export namespace obscure::vulkan
{
    struct swap_chain_data : vk::SwapchainKHR {
        vk::Format format;
        vk::Extent2D extent;
        msaa_buffer multisaa_buffer;
        depth_buffer depth_buffer_;
        vk::RenderPass render_pass;
        uint32_t current_frame_index;
        uint32_t image_count;
        uint32_t current_frame;
        std::array<vk::Image, max_image_count()> images;
        std::array<image_view, max_image_count()> image_views;
        std::array<vk::Framebuffer, max_image_count()> framebuffers;
        std::array<obscure::vulkan::semaphore, max_image_count()> image_available;
        std::array<obscure::vulkan::semaphore, max_image_count()> render_finished;
        std::array<obscure::vulkan::fence, max_image_count()> in_flight;

    private:
        static vk::SurfaceFormatKHR
        get_swap_format(vk::PhysicalDevice device, vk::SurfaceKHR surface)
        {
            auto available_formats = device.getSurfaceFormatsKHR(surface);
            for (auto format : available_formats) {
                if (format.format == vk::Format::eR8G8B8A8Srgb &&
                    format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                    return format;
                }
            }

            return available_formats[0];
        }

        static vk::PresentModeKHR
        get_present_mode(vk::PhysicalDevice device, vk::SurfaceKHR surface)
        {
            for (auto present_mode : device.getSurfacePresentModesKHR(surface)) {
                if (present_mode == vk::PresentModeKHR::eMailbox) {
                    return vk::PresentModeKHR::eMailbox;
                }
            }
            return vk::PresentModeKHR::eFifo;
        }

        static vk::Extent2D
        get_extent(vk::PhysicalDevice device, vk::SurfaceKHR surface, obscure::glfw::glfw_window_ref window)
        {
            auto capabilities = device.getSurfaceCapabilitiesKHR(surface);
            auto current_extent = capabilities.currentExtent;
            if (current_extent.width != std::numeric_limits<uint32_t>::max()) {
                return current_extent;
            } else {
                current_extent = window.get_window_extent();
                current_extent.width = std::clamp(
                    current_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                current_extent.height = std::clamp(
                    current_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
                return current_extent;
            }
        }

        static vk::RenderPass
        create_render_pass(vk::Device device,
                           vk::Format color_format,
                           vk::Format depth_format,
                           vk::SampleCountFlagBits samples)
        {
            vk::AttachmentDescription color_attachment{ {},
                                                        color_format,
                                                        samples,
                                                        vk::AttachmentLoadOp::eClear,
                                                        vk::AttachmentStoreOp::eStore,
                                                        vk::AttachmentLoadOp::eDontCare,
                                                        vk::AttachmentStoreOp::eDontCare,
                                                        vk::ImageLayout::eUndefined,
                                                        vk::ImageLayout::eColorAttachmentOptimal };

            vk::AttachmentReference color_attachment_ref{ 0, vk::ImageLayout::eColorAttachmentOptimal };

            vk::AttachmentDescription depth_attachment{ {},
                                                        depth_format,
                                                        samples,
                                                        vk::AttachmentLoadOp::eClear,
                                                        vk::AttachmentStoreOp::eDontCare,
                                                        vk::AttachmentLoadOp::eDontCare,
                                                        vk::AttachmentStoreOp::eDontCare,
                                                        vk::ImageLayout::eUndefined,
                                                        vk::ImageLayout::eDepthStencilAttachmentOptimal };

            vk::AttachmentReference depth_attachment_ref{ 1, vk::ImageLayout::eDepthStencilAttachmentOptimal };

            vk::AttachmentDescription color_resolve_attachment{ {},
                                                                color_format,
                                                                vk::SampleCountFlagBits::e1,
                                                                vk::AttachmentLoadOp::eDontCare,
                                                                vk::AttachmentStoreOp::eStore,
                                                                vk::AttachmentLoadOp::eDontCare,
                                                                vk::AttachmentStoreOp::eDontCare,
                                                                vk::ImageLayout::eUndefined,
                                                                vk::ImageLayout::ePresentSrcKHR };

            vk::AttachmentReference color_resolve_attachment_ref{ 2, vk::ImageLayout::eColorAttachmentOptimal };

            vk::SubpassDescription subpass{ {},
                                            vk::PipelineBindPoint::eGraphics,
                                            0,
                                            nullptr,
                                            1,
                                            &color_attachment_ref,
                                            &color_resolve_attachment_ref,
                                            &depth_attachment_ref,
                                            0,
                                            nullptr };

            vk::SubpassDependency dependency{
                VK_SUBPASS_EXTERNAL,
                0,
                vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                {},
                vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                {}
            };

            vk::AttachmentDescription attachments[3] = { color_attachment, depth_attachment, color_resolve_attachment };
            vk::RenderPassCreateInfo create_info{ {}, 3, attachments, 1, &subpass, 1, &dependency };

            return device.createRenderPass(create_info);
        }

        static vk::SwapchainKHR
        create_swap_chain(obscure::vulkan::device const& device,
                          vk::SurfaceKHR surface,
                          vk::SurfaceFormatKHR format,
                          vk::PresentModeKHR present_mode,
                          vk::Extent2D extent)
        {
            auto surface_capabilities = device.physical_device.getSurfaceCapabilitiesKHR(surface);
            uint32_t image_count = surface_capabilities.minImageCount + 1;
            if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount) {
                image_count = surface_capabilities.maxImageCount;
            }

            std::array<uint32_t, 2> queue_indices{ device.graphics_queue_index, device.present_queue_index };
            bool concurrent = device.graphics_queue_index != device.present_queue_index;
            vk::SharingMode sharing_mode = concurrent ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;

            vk::SwapchainCreateInfoKHR create_info{ {},
                                                    surface,
                                                    image_count,
                                                    format.format,
                                                    format.colorSpace,
                                                    extent,
                                                    1,
                                                    vk::ImageUsageFlagBits::eColorAttachment,
                                                    sharing_mode,
                                                    queue_indices,
                                                    surface_capabilities.currentTransform,
                                                    vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                    present_mode,
                                                    VK_TRUE };

            return device.createSwapchainKHR(create_info);
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
        // your code for which the warning gets suppressed

        template<std::size_t... Idxs>
        static inline std::array<obscure::vulkan::semaphore, sizeof...(Idxs)>
        initialize_semaphores(vk::Device device, std::index_sequence<Idxs...>)
        {
            return std::array<obscure::vulkan::semaphore, sizeof...(Idxs)>{ (Idxs,
                                                                             obscure::vulkan::semaphore{ device })... };
        }

        template<std::size_t... Idxs>
        static inline std::array<obscure::vulkan::fence, sizeof...(Idxs)>
        initialize_fences(vk::Device device, bool Signaled, std::index_sequence<Idxs...>)
        {
            return std::array<obscure::vulkan::fence, sizeof...(Idxs)>{ (
                Idxs, obscure::vulkan::fence{ device, Signaled })... };
        }
#pragma clang diagnostic pop

        static inline std::array<obscure::vulkan::semaphore, max_image_count()>
        initialize_semaphores(vk::Device device)
        {
            return initialize_semaphores(device, std::make_index_sequence<max_image_count()>{});
        }

        static inline std::array<obscure::vulkan::fence, max_image_count()>
        initialize_fences(vk::Device device, bool Signaled)
        {
            return initialize_fences(device, Signaled, std::make_index_sequence<max_image_count()>{});
        }

        static inline std::array<vk::Image, max_image_count()>
        load_images(vk::Device device, vk::SwapchainKHR swapchain, uint32_t& image_count)
        {
            std::array<vk::Image, max_image_count()> result{};
            auto err0 = device.getSwapchainImagesKHR(swapchain, &image_count, nullptr);
            auto err1 = device.getSwapchainImagesKHR(swapchain, &image_count, result.data());
            return result;
        }

        static inline std::array<image_view, max_image_count()>
        load_image_views(std::array<vk::Image, max_image_count()> const& images, vk::Device device, vk::Format format)
        {
            std::array<image_view, max_image_count()> result{};
            for (std::size_t idx = 0; idx < max_image_count(); ++idx) {
                result[idx] = image_view{ device, images[idx], format };
            }
            return result;
        }

        static inline std::array<vk::Framebuffer, max_image_count()>
        load_framebuffers(std::array<image_view, max_image_count()> const& image_views,
                          depth_buffer const& depth_buffer_,
                          msaa_buffer const& color_buffer,
                          vk::Device device,
                          vk::RenderPass render_pass,
                          vk::Extent2D extent)
        {
            std::array<vk::Framebuffer, max_image_count()> result{};
            for (std::size_t idx = 0; idx < max_image_count(); ++idx) {
                if (image_views[idx] != VK_NULL_HANDLE) {
                    vk::ImageView views[3] = { color_buffer.sample_image_views[idx],
                                               depth_buffer_.views[idx],
                                               image_views[idx] };
                    vk::FramebufferCreateInfo create_info{ {}, render_pass, 3, views, extent.width, extent.height, 1 };
                    result[idx] = device.createFramebuffer(create_info);
                } else {
                    result[idx] = VK_NULL_HANDLE;
                }
            }
            return result;
        }

        swap_chain_data(obscure::vulkan::device const& device,
                        vk::SurfaceKHR _surface,
                        vk::SurfaceFormatKHR _format,
                        vk::PresentModeKHR _present_mode,
                        vk::Extent2D _extent)
            : vk::SwapchainKHR(create_swap_chain(device, _surface, _format, _present_mode, _extent))
            , format(_format.format)
            , extent(_extent)
            , multisaa_buffer(device, format, extent)
            , depth_buffer_(device, vk::Extent3D{ extent, 1 }, multisaa_buffer.msaa_samples)
            , render_pass(
                  create_render_pass(device.get_device(), format, depth_buffer_.format, multisaa_buffer.msaa_samples))
            , current_frame_index()
            , image_count()
            , current_frame()
            , images(load_images(device.get_device(), get_swap_chain(), image_count))
            , image_views(load_image_views(images, device.get_device(), format))
            , framebuffers(load_framebuffers(image_views,
                                             depth_buffer_,
                                             multisaa_buffer,
                                             device.get_device(),
                                             render_pass,
                                             extent))
            , image_available(initialize_semaphores(device.get_device()))
            , render_finished(initialize_semaphores(device.get_device()))
            , in_flight(initialize_fences(device.get_device(), true))
        {
        }

    public:
        swap_chain_data(obscure::vulkan::device const& device,
                        vk::SurfaceKHR _surface,
                        obscure::glfw::glfw_window_ref window)
            : swap_chain_data(device,
                              _surface,
                              get_swap_format(device.physical_device, _surface),
                              get_present_mode(device.physical_device, _surface),
                              get_extent(device.physical_device, _surface, window))
        {
        }

        [[nodiscard]] vk::SwapchainKHR
        get_swap_chain() const&
        {
            return static_cast<vk::SwapchainKHR>(*this);
        }

        [[nodiscard]] vk::SwapchainKHR const*
        get_swap_chain_ptr() const&
        {
            return this;
        }

        [[nodiscard]] std::size_t
        get_current_frame() const noexcept
        {
            return current_frame;
        }

        [[nodiscard]] uint32_t
        get_frame_index() const noexcept
        {
            return current_frame_index;
        }

        [[nodiscard]] uint32_t const*
        get_frame_index_ptr() const noexcept
        {
            return &current_frame_index;
        }

        [[nodiscard]] std::size_t
        get_frame_count() const noexcept
        {
            return image_count;
        }

        [[nodiscard]] vk::Format
        get_format() const noexcept
        {
            return format;
        }
        [[nodiscard]] vk::Extent2D
        get_extent() const noexcept
        {
            return extent;
        }
        [[nodiscard]] vk::RenderPass
        get_render_pass() const noexcept
        {
            return render_pass;
        }

        [[nodiscard]] vk::Semaphore const*
        get_image_available_ptr() const noexcept
        {
            return image_available[current_frame].get_semaphore_ptr();
        }

        [[nodiscard]] vk::Semaphore const*
        get_render_finished_ptr() const noexcept
        {
            return render_finished[current_frame_index].get_semaphore_ptr();
        }

        [[nodiscard]] vk::Fence
        get_in_flight_fence() const noexcept
        {
            return in_flight[current_frame].get_fence();
        }
    };

    using swap_chain_ref = std::reference_wrapper<const swap_chain_data>;

    struct swap_chain : swap_chain_data {
        std::reference_wrapper<const device> vk_device;

    private:
        [[nodiscard]] vk::Device
        get_device() const noexcept
        {
            return vk_device.get().get_device();
        }

    public:
        swap_chain(device const& _device, vk::SurfaceKHR _surface, obscure::glfw::glfw_window_ref window)
            : swap_chain_data(_device, _surface, window)
            , vk_device(_device)
        {
        }

        [[nodiscard]] uint32_t
        get_next_frame_index()
        {
            current_frame = (current_frame + 1) % get_frame_count();

            in_flight[current_frame].wait_and_reset(get_device());
            vkAcquireNextImageKHR(get_device(),
                                  get_swap_chain(),
                                  std::numeric_limits<uint64_t>::max(),
                                  image_available[current_frame].get_semaphore(),
                                  VK_NULL_HANDLE,
                                  &current_frame_index);
            return current_frame_index;
        }

        ~swap_chain() noexcept
        {
            for (auto& semaphore : image_available) {
                semaphore.free(get_device());
            }

            for (auto& semaphore : render_finished) {
                semaphore.free(get_device());
            }

            for (auto& fence : in_flight) {
                fence.free(get_device());
            }

            for (auto buffer : framebuffers) {
                get_device().destroyFramebuffer(buffer);
            }

            for (auto image_view : image_views) {
                image_view.free(get_device());
            }
            get_device().destroyRenderPass(render_pass);
            depth_buffer_.free(vk_device);
            multisaa_buffer.free(vk_device);
            get_device().destroySwapchainKHR(get_swap_chain());
        }

        operator swap_chain_ref() const&
        {
            swap_chain_data const& data = *this;
            return std::ref(data);
        }
    };
} // namespace obscure::vulkan