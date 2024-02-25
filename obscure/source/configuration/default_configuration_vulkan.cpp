module;
#include <compare>
module obscure.configuration.default_configuration:vulkan;
import obscure.vulkan.device_picker;
import obscure.vulkan.verbose_console_logger;
import obscure.vulkan.verbose_csv_logger;
import obscure.properties;

VkPhysicalDevice obscure::configuration::default_vulkan_configuration::pick_device(obscure::internal::vulkan::instance instance, obscure::internal::vulkan::surface surface)
{
	obscure::vulkan::device_picker picker(instance, surface);
	return picker.pick_device();
}

obscure::vulkan::logger_collection obscure::configuration::default_vulkan_configuration::vulkan_logging(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator)
{
	if constexpr (obscure::enable_debug_features())
	{
		return obscure::vulkan::logger_collection
		{
			new obscure::vulkan::verbose_console_logger{instance, allocator},
			new obscure::vulkan::verbose_csv_logger{instance, "vulkan_log.csv", allocator}
		};
	}
	else
	{
		return obscure::vulkan::logger_collection{};
	}
}