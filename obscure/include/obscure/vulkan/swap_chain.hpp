#ifndef OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/vulkan/device.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/image_view.hpp"
#include "obscure/vulkan/semaphore.hpp"
#include "obscure/vulkan/fence.hpp"
#include "obscure/helper_templates/vector_initialize.hpp"
#include <vector>
#include <limits>

namespace obscure
{
	namespace vulkan
	{
		struct swap_chain_data : vk::SwapchainKHR
		{
			vk::Format format;
			vk::Extent2D extent;
			vk::RenderPass render_pass;
			std::vector<vk::Image> images;
			std::vector<image_view> image_views;
			std::vector<vk::Framebuffer> framebuffers;

			std::vector<obscure::vulkan::semaphore> image_available;
			std::vector<obscure::vulkan::semaphore> render_finished;
			std::vector<obscure::vulkan::fence> in_flight;
			std::size_t current_frame;
			uint32_t current_frame_index;


		private:
			static vk::SurfaceFormatKHR get_swap_format(vk::PhysicalDevice device, vk::SurfaceKHR surface)
			{
				auto available_formats = device.getSurfaceFormatsKHR(surface);
				for (auto format : available_formats)
				{
					if (format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
					{
						return format;
					}
				}

				return available_formats[0];
			}

			static vk::PresentModeKHR get_present_mode(vk::PhysicalDevice device, vk::SurfaceKHR surface)
			{
				for (auto present_mode : device.getSurfacePresentModesKHR(surface))
				{
					if (present_mode == vk::PresentModeKHR::eMailbox)
					{
						return vk::PresentModeKHR::eMailbox;
					}
				}
				return vk::PresentModeKHR::eFifo;
			}

			static vk::Extent2D get_extent(vk::PhysicalDevice device, vk::SurfaceKHR surface, obscure::glfw::glfw_window_ref window)
			{
				auto capabilities = device.getSurfaceCapabilitiesKHR(surface);
				auto current_extent = capabilities.currentExtent;
				if (current_extent.width != std::numeric_limits<uint32_t>::max())
				{
					return current_extent;
				}
				else
				{
					current_extent = window.get_window_extent();
					current_extent.width = std::clamp(current_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
					current_extent.height = std::clamp(current_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
					return current_extent;
				}
			}

			static vk::RenderPass create_render_pass(vk::Device device, vk::Format format)
			{
				vk::AttachmentDescription color_attachment {
	                        {},
							format,
							vk::SampleCountFlagBits::e1,
							vk::AttachmentLoadOp::eClear,
							vk::AttachmentStoreOp::eStore,
							vk::AttachmentLoadOp::eDontCare,
							vk::AttachmentStoreOp::eDontCare,
							vk::ImageLayout::eUndefined,
							vk::ImageLayout::ePresentSrcKHR
						};

				vk::AttachmentReference color_attachment_ref {
					0,
					vk::ImageLayout::eColorAttachmentOptimal
				};

				vk::SubpassDescription subpass {
	                        {},
							vk::PipelineBindPoint::eGraphics,
							0,
							nullptr,
							1,
							&color_attachment_ref,
							nullptr,
							nullptr,
							0,
							nullptr
						};

				vk::SubpassDependency dependency {
					VK_SUBPASS_EXTERNAL,
					0,
					vk::PipelineStageFlagBits::eColorAttachmentOutput,
					vk::PipelineStageFlagBits::eColorAttachmentOutput,
					{},
					vk::AccessFlagBits::eColorAttachmentWrite,
					{}
				};

				vk::RenderPassCreateInfo create_info {
	                        {},
							1,
							&color_attachment,
							1,
							&subpass,
							1,
							&dependency
						};

				return device.createRenderPass(create_info);
			}

			static vk::SwapchainKHR create_swap_chain(obscure::vulkan::device const& device, vk::SurfaceKHR surface, vk::SurfaceFormatKHR format, vk::PresentModeKHR present_mode, vk::Extent2D extent)
			{
				auto surface_capabilities = device.physical_device.getSurfaceCapabilitiesKHR(surface);
				uint32_t image_count = surface_capabilities.minImageCount + 1;
				if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount)
				{
					image_count = surface_capabilities.maxImageCount;
				}

				std::array<uint32_t, 2> queue_indices{ device.graphics_queue_index, device.present_queue_index };
				bool concurrent = device.graphics_queue_index != device.present_queue_index;
				vk::SharingMode sharing_mode = concurrent ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive;

				vk::SwapchainCreateInfoKHR create_info
				{
					{},
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
					VK_TRUE
				};

				return device.createSwapchainKHR(create_info);

			}

			static inline std::vector<obscure::vulkan::semaphore> initialize_semaphores(std::size_t size, vk::Device device)
			{
				return obscure::helper_templates::initialize_vector<obscure::vulkan::semaphore>(size, device);
			}

			static inline std::vector<obscure::vulkan::fence> initialize_fences(std::size_t size, vk::Device device, bool Signaled)
			{
				return obscure::helper_templates::initialize_vector<obscure::vulkan::fence>(size, device, Signaled);
			}

			swap_chain_data(obscure::vulkan::device const& device, vk::SurfaceKHR _surface, vk::SurfaceFormatKHR _format, vk::PresentModeKHR _present_mode, vk::Extent2D _extent)
				: vk::SwapchainKHR(create_swap_chain(device, _surface, _format, _present_mode, _extent)),
				format(_format.format),
				extent(_extent),
				render_pass(create_render_pass(device.get_device(), format)),
				images(device.getSwapchainImagesKHR(get_swap_chain())),
				image_views(),
				framebuffers(),
				image_available(initialize_semaphores(images.size(), device.get_device())),
				render_finished(initialize_semaphores(images.size(), device.get_device())),
				in_flight(initialize_fences(images.size(), device.get_device(), true)),
				current_frame(),
				current_frame_index()
			{
				image_views.reserve(images.size());
				for (auto image : images)
				{
					image_views.emplace_back(device, image, format);
				}

				framebuffers.reserve(images.size());
				for (auto image : image_views)
				{
					vk::FramebufferCreateInfo create_info {
						{},
						render_pass,
						1,
						&image,
						extent.width,
						extent.height,
						1
					};

					framebuffers.push_back(device.createFramebuffer(create_info));
				}
			}

		public:
			swap_chain_data(obscure::vulkan::device const& device, vk::SurfaceKHR _surface, obscure::glfw::glfw_window_ref window)
			: swap_chain_data(device, _surface, get_swap_format(device.physical_device, _surface),
				get_present_mode(device.physical_device, _surface),
				get_extent(device.physical_device, _surface, window))
			{}

			[[nodiscard]] vk::SwapchainKHR get_swap_chain() const&
			{
				return static_cast<vk::SwapchainKHR>(*this);
			}

			[[nodiscard]] vk::SwapchainKHR const* get_swap_chain_ptr() const&
			{
				return this;
			}

			[[nodiscard]] std::size_t get_current_frame() const noexcept {
				return current_frame;
			}

			[[nodiscard]] uint32_t get_frame_index() const noexcept
			{
				return current_frame_index;
			}

			[[nodiscard]] uint32_t const* get_frame_index_ptr() const noexcept
			{
				return &current_frame_index;
			}

			[[nodiscard]] std::size_t get_frame_count() const noexcept
			{
				return images.size();
			}

			[[nodiscard]] vk::Format     get_format() const noexcept
			{
			    return format;
			}
			[[nodiscard]] vk::Extent2D   get_extent() const noexcept
			{
			    return extent;
			}
			[[nodiscard]] vk::RenderPass get_render_pass() const noexcept
			{
			    return render_pass;
			}

			[[nodiscard]] vk::Semaphore const* get_image_available_ptr() const noexcept {
				return image_available[current_frame].get_semaphore_ptr();
			}

			[[nodiscard]] vk::Semaphore const* get_render_finished_ptr() const noexcept {
				return render_finished[current_frame].get_semaphore_ptr();
			}

			[[nodiscard]] vk::Fence get_in_flight_fence() const noexcept {
				return in_flight[current_frame].get_fence();
			}
		};

		using swap_chain_ref = std::reference_wrapper<const swap_chain_data>;


		struct swap_chain : swap_chain_data, protected virtual vk::Device
		{

		private:
			[[nodiscard]] vk::Device get_device() const& noexcept
			{
				return static_cast<vk::Device>(*this);
			}

		public:

			swap_chain(device const& _device, vk::SurfaceKHR _surface, obscure::glfw::glfw_window_ref window)
				: swap_chain_data(_device, _surface, window), vk::Device(_device.get_device())
			{}

			[[nodiscard]] uint32_t get_next_frame_index()
			{
				current_frame = (current_frame + 1) % get_frame_count();

				in_flight[current_frame].wait_and_reset(get_device());
				vkAcquireNextImageKHR(get_device(), get_swap_chain(), std::numeric_limits<uint64_t>::max(), image_available[current_frame].get_semaphore(), VK_NULL_HANDLE, &current_frame_index);
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


				for (auto buffer : framebuffers)
				{
					get_device().destroyFramebuffer(buffer);
				}

				for (auto image_view : image_views)
				{
					image_view.free(get_device());
				}
				get_device().destroyRenderPass(render_pass);
				get_device().destroySwapchainKHR(get_swap_chain());
			}


			operator swap_chain_ref() const& {
				swap_chain_data const& data = *this;
				return std::ref(data);
			}
		};
	}
}

#endif