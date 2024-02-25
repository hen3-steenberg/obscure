module obscure.vulkan.verbose_logger;

obscure::vulkan::verbose_logger::verbose_logger(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator)
	: logger(instance, any_severity, all_message_types, allocator)
{}