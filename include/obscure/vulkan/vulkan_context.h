#ifndef OBSCURE_VULKAN_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_CONTEXT_DEFINITION 1

#include <memory>
#include "obscure/vulkan/application_context.h"

namespace obscure
{
	namespace vulkan
	{
		struct vulkan_context
		{
			std::shared_ptr<application_context> data;

			vulkan_context() noexcept;
			vulkan_context(obscure::configuration::configuration_provider * configuration)
				: data(std::make_shared<application_context>(configuration))
			{}

			void poll_events() const noexcept;
			bool should_close() const noexcept;
		};
	}
}

#endif