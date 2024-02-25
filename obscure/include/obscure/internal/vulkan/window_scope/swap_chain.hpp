module;

#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.swap_chain;
export import :configuration;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.render_pass;
import obscure.internal.vulkan.image_view;
import obscure.internal.vulkan.frame_buffer;
import obscure.internal.vulkan.semaphore;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			struct sc_data;

			export struct swap_chain
			{
			private:	
				sc_data* data;
			public:
				swap_chain() noexcept;
				swap_chain(const swap_chain & other) = delete;
				swap_chain(swap_chain& other) noexcept;
				swap_chain(swap_chain&& other) noexcept;

				swap_chain(vulkan::device _device, surface _surface, VkExtent2D _native_resolution);

				swap_chain& operator=(swap_chain&& other) = default;

				VkSwapchainKHR get_handle() const noexcept;
				VkSwapchainKHR const* get_handle_address() const noexcept;

				uint32_t get_next_image_index(semaphore image_ready_signal);

				~swap_chain() noexcept;
			};
		}
	}
}