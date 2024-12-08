#ifndef OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/vulkan/device.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/vulkan/image_view.hpp"
#include <vector>

namespace obscure
{
	namespace vulkan
	{
		template<typename ... TSiblings>
		struct swap_chain : vk::SwapchainKHR
		{
			vk::Format format;
			vk::Extent2D extent;
			std::vector<vk::Image> images;
			std::vector<image_view> image_views;

			obscure::vulkan::device& get_parent_ref()&
			{
				return obscure::helper_templates::get_parent_ref<obscure::vulkan::device, TSiblings...>(this);
			}

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

			static vk::Extent2D get_extent(vk::PhysicalDevice device, vk::SurfaceKHR surface, obscure::glfw::glfw_window const& window)
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

			static vk::SwapchainKHR create_swap_chain(obscure::vulkan::device& device, vk::SurfaceKHR surface, vk::SurfaceFormatKHR format, vk::PresentModeKHR present_mode, vk::Extent2D extent)
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

			swap_chain(vk::SurfaceKHR _surface, vk::SurfaceFormatKHR _format, vk::PresentModeKHR _present_mode, vk::Extent2D _extent)
				: vk::SwapchainKHR(create_swap_chain(get_parent_ref(), _surface, _format, _present_mode, _extent)),
				format(_format.format),
				extent(_extent),
				images(get_parent_ref().getSwapchainImagesKHR(*this)),
				image_views()
			{
				image_views.reserve(images.size());
				for (auto image : images)
				{
					image_views.emplace_back(get_parent_ref(), image, format);
				}
			}

		public:

			swap_chain(vk::SurfaceKHR _surface, obscure::glfw::glfw_window const& window)
				: swap_chain(_surface, get_swap_format(get_parent_ref().physical_device, _surface),
					get_present_mode(get_parent_ref().physical_device, _surface),
					get_extent(get_parent_ref().physical_device, _surface, window))
			{}


			~swap_chain() noexcept
			{
				for (auto image_view : image_views)
				{
					image_view.free(get_parent_ref());
				}
				get_parent_ref().destroySwapchainKHR(*this);
			}
			

			
		};
	}
}

#endif