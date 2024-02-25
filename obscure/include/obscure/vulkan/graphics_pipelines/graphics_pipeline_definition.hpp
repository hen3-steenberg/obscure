export module obscure.vulkan.graphics_pipeline_definition;
export import obscure.vulkan.graphics_pipeline_configuration;
export import obscure.vulkan.graphics_pipeline_context;

namespace obscure
{
	namespace vulkan
	{
		export template<typename T>
		concept pipeline_definition = requires
		{
			typename T::configuration;
				requires pipeline_configuration<typename T::configuration>;
			typename T::render_context;
				requires pipeline_context<typename T::render_context>;
		};
	}
}