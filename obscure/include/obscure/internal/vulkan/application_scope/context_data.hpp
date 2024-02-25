module;
export module obscure.internal.vulkan.context_data;
import obscure.internal.vulkan.glfw_window;
import obscure.internal.vulkan.instance;
import obscure.vulkan.logger_collection;
import obscure.internal.vulkan.surface;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.swap_chain;
import obscure.internal.vulkan.command_pool;
import obscure.internal.vulkan.frame_data;
import obscure.configuration;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			struct context_data
			{
				glfw_window window;
				instance instance;
				obscure::vulkan::logger_collection loggers;
				frame_data frame_data;
				command_pool transfer_command_pool;

				context_data(obscure::configuration::application_configuration* app_configuration, obscure::configuration::vulkan_configuration* vulkan_config);
				~context_data() noexcept;

				device & get_device()&;
				device const& get_device() const&;

				swap_chain& get_swap_chain()&;
				swap_chain const& get_swap_chain() const&;


				void wait_for_device_idle()&;
			};
		}
	}
}