module;
#include <vector>
#include <atomic>
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.swap_chain;
import :configuration;
import obscure.internal.vulkan.vulkan_load;
import obscure.memory.circular_buffer;
import obscure.properties;

namespace obscure
{

	namespace internal
	{
		namespace vulkan
		{
			struct sc_data
			{
			private:
				static obscure::memory::circular_buffer<obscure::maximum_frames_in_flight() * 2> fast_allocator;
			public:
				device device;
				VkSwapchainKHR vk_swap_chain;
				VkFormat image_format;
				VkExtent2D extent;
				render_pass render_pass;
				std::vector<VkImage> swap_chain_images;
				std::vector<image_view> swap_chain_views;
				std::vector<frame_buffer> frame_buffers;
				std::atomic_uint64_t reference_count;

				sc_data() = delete;
				sc_data(sc_data const&) = delete;
				sc_data(obscure::internal::vulkan::surface _surface, VkExtent2D _native_resolution, obscure::internal::vulkan::device _device)
					: reference_count(1)
				{
					obscure::internal::vulkan::swap_chain_configuration config{ device.physical_device, _surface, _native_resolution, device.graphics_queue_index, device.present_queue_index };

					VkSwapchainCreateInfoKHR create_info = config.parse_configuration();

					device = device;
					image_format = create_info.imageFormat;
					extent = create_info.imageExtent;

					render_pass = obscure::internal::vulkan::render_pass(device, _this->image_format, nullptr);

					VkResult Err = vkCreateSwapchainKHR(device.get_handle(), &create_info, nullptr, &_this->vk_swap_chain);
					if (Err != VK_SUCCESS) throw Err;

					swap_chain_images = vulkan_load(device.get_handle(), _this->vk_swap_chain, vkGetSwapchainImagesKHR);

					swap_chain_views.reserve(_this->swap_chain_images.size());

					for (auto image : _this->swap_chain_images)
					{
						swap_chain_views.emplace_back(device, image, _this->image_format);
					}

					frame_buffers.reserve(_this->swap_chain_images.size());

					for (auto view : _this->swap_chain_views)
					{
						frame_buffers.emplace_back(device, view, _this->render_pass, _this->extent, nullptr);
					}
					
				}

				void add_reference()& noexcept
				{
					++reference_count;
				}

				bool remove_reference()& noexcept
				{
					return reference_count.fetch_sub(1) == 1;
				}

				~sc_data() noexcept
				{
					for (auto& buffer : frame_buffers)
					{
						buffer.free(device, nullptr);
					}

					for (auto& view : swap_chain_views)
					{
						view.free(device, nullptr);
					}

					render_pass.free(device, nullptr);

					vkDestroySwapchainKHR(device.get_handle(), vk_swap_chain, nullptr);
				}

				void* operator new (size_t size)
				{
					return fast_allocator.allocate(size);
				}

				void operator delete(void* p)
				{
					return fast_allocator.deallocate(p);
				}
			};
		}
	}
}


obscure::internal::vulkan::swap_chain::swap_chain() noexcept
	: data(nullptr)
{}

obscure::internal::vulkan::swap_chain::swap_chain(swap_chain& other) noexcept
	: data(other.data)
{
	if (data)
	{
		data->add_reference();
	}
}

obscure::internal::vulkan::swap_chain::swap_chain(swap_chain&& other)
	: data(other.data)
{
	other.data = nullptr;
}


obscure::internal::vulkan::swap_chain::swap_chain(device device, surface _surface, VkExtent2D _native_resolution)
	: data(new sc_data{ _surface, _native_resolution, device })
{
}

VkSwapchainKHR obscure::internal::vulkan::swap_chain::get_handle() const noexcept
{
	if (data)
	{
		return data->vk_swap_chain;
	}
	else
	{
		return VK_NULL_HANDLE;
	}
	
}

VkSwapchainKHR const* obscure::internal::vulkan::swap_chain::get_handle_address() const noexcept
{
	if (data)
	{
		return &data->vk_swap_chain;
	}
	else
	{
		return nullptr;
	}
}

obscure::internal::vulkan::swap_chain::~swap_chain() noexcept
{
	if (data && data->remove_reference())
	{
		delete data;
	}
}


void obscure::internal::vulkan::swap_chain::recreate(device device, surface _surface, VkExtent2D _native_resolution, VkAllocationCallbacks const* allocator)
{
	device.wait_for_idle();
	free(device, allocator);
	frame_buffers.clear();
	swap_chain_views.clear();
	swap_chain_images.clear();

	initialize_swap_chain(this, _physical_device, _surface, _native_resolution, device, allocator);
	*resize_requested = false;
}

uint32_t obscure::internal::vulkan::swap_chain::get_next_image_index(semaphore image_ready_signal)
{
	if (data)
	{
		uint32_t imageIndex;
		vkAcquireNextImageKHR(data->device.get_handle(), data->vk_swap_chain, UINT64_MAX, image_ready_signal.get_handle(), VK_NULL_HANDLE, &imageIndex);
		return imageIndex;
	}
	else
	{
		return -1;
	}
}