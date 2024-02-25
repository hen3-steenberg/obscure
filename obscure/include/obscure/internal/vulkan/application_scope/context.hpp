export module obscure.internal.vulkan.context;
import obscure.vulkan.graphics_pipeline_definition;
import obscure.internal.vulkan.context_data;
import obscure.internal.vulkan.pipeline_collection;
import obscure.internal.vulkan.initialize_pipeline_collection;
import obscure.configuration;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			template<obscure::vulkan::pipeline_definition ... Tpipelines>
			struct context
			{
				context_data data;
				pipeline_collection<sizeof...(Tpipelines)> pipeline_data;

				context(obscure::configuration::configuration_container auto config)
					: data(config->load_application_configuration(), config->load_vulkan_configuration())
				{
					pipeline_data = initialize_pipeline_collection<Tpipelines::configuration...>(data.get_device(), data.get_swap_chain(), nullptr);
				}

				~context() noexcept
				{
					data.wait_for_device_idle();
					pipeline_data.free(data.get_device());
				}

			};
		}
	}
}