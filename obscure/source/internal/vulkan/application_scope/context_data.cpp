module;

module obscure.internal.vulkan.context_data;

obscure::internal::vulkan::context_data::context_data(obscure::configuration::application_configuration* app_configuration, obscure::configuration::vulkan_configuration* vulkan_config)
	:	window(app_configuration), 
		instance(app_configuration->application_title(), 
		app_configuration->application_version()),
		loggers(vulkan_config->vulkan_logging(instance)),
		frame_data(vulkan_config->pick_device(instance, surface), surface(instance, window), window.get_native_resolution()),
		transfer_command_pool(command_pool::create_transfer_pool(get_device()))
{
	if (app_configuration->allow_resizing())
	{
		window.enable_resizing(&frame_data.should_resize);
	}
}

obscure::internal::vulkan::context_data::~context_data() noexcept
{
	transfer_command_pool.free(get_device());
	frame_data.free(instance);
	loggers.free(instance);
	instance.free();
	window.free();
}

void obscure::internal::vulkan::context_data::wait_for_device_idle()&
{
	frame_data.wait_for_device_idle();
}

device& obscure::internal::vulkan::context_data::get_device()&
{
	return frame_data.device;
}

device const& obscure::internal::vulkan::context_data::get_device() const&
{
	return frame_data.device;
}

swap_chain& obscure::internal::vulkan::context_data::get_swap_chain()&
{
	return frame_data.swap_chain;
}

swap_chain const& obscure::internal::vulkan::context_data::get_swap_chain() const&
{
	return frame_data.swap_chain;
}