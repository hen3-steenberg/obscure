#ifndef OBSCURE_VULKAN_APPLICATION_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_APPLICATION_CONTEXT_DEFINITION 1

#include "obscure/vulkan/window.h"
#include "obscure/vulkan/instance.h"
#include "obscure/vulkan/surface.h"
#include "obscure/vulkan/device.h"
#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/pipeline_collection.h"
#include "obscure/vulkan/logger_collection.h"
#include "obscure/vulkan/command_pool.h"
#include "obscure/vulkan/fence.h"
#include "obscure/properties.hpp"
#include <array>

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
			VkQueue graphics_queue;
			VkQueue present_queue;
			vulkan::swap_chain swap_chain;
			vulkan::pipeline_collection pipelines;			
			vulkan::command_pool graphics_command_pool;
			std::array<VkCommandBuffer, maximum_frames_in_flight()> graphics_command_buffers;
			std::array<vulkan::fence, maximum_frames_in_flight()>  draw_complete_fences;
			std::array<semaphore, maximum_frames_in_flight()>  ready_to_draw_semaphores;
			std::array<semaphore, maximum_frames_in_flight()>  ready_to_display_semaphores;
			uint32_t current_frame;

			application_context(obscure::configuration::configuration_provider* configuration);


			~application_context();
		};
	}
}
#endif