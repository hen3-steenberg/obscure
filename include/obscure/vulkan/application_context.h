#ifndef OBSCURE_VULKAN_APPLICATION_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_APPLICATION_CONTEXT_DEFINITION 1

#include "obscure/vulkan/window.h"
#include "obscure/vulkan/instance.h"
#include "obscure/vulkan/surface.h"
#include "obscure/vulkan/device.h"
#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/pipeline_collection.h"
#include "obscure/vulkan/logger_collection.h"

namespace obscure
{
	namespace configuration
	{
		struct configuration_provider;
	}

	namespace vulkan
	{
		

		struct application_context
		{
			//VkAllocationCallbacks allocator;
			vulkan::glfw_window window;
			vulkan::instance instance;
			vulkan::logger_collection loggers;
			vulkan::surface surface;
			VkPhysicalDevice physical_device;
			vulkan::device device;
			vulkan::swap_chain swap_chain;
			vulkan::pipeline_collection pipelines;			

			application_context(obscure::configuration::configuration_provider* configuration);


			~application_context();
		};
	}
}
#endif