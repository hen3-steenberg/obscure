export module obscure.configuration.default_configuration:vulkan;
export import obscure.configuration.vulkan_configuration;

namespace obscure
{
	namespace configuration
	{
		export struct default_vulkan_configuration : vulkan_configuration
		{
			virtual VkPhysicalDevice pick_device(obscure::internal::vulkan::instance instance, obscure::internal::vulkan::surface surface) override;

			virtual vulkan::logger_collection vulkan_logging(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr) override;

			virtual ~default_vulkan_configuration() = default;
		};
	}
}