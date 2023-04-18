#ifndef OBSCURE_VULKAN_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_CONTEXT_DEFINITION 1

#include <memory>
#include <initializer_list>
#include <algorithm>
#include "obscure/vulkan/application_context.h"
#include "obscure/vulkan/command_sequence.h"
#include "obscure/vulkan/owning_buffer.h"
#include "obscure/vulkan/vertex_array.hpp"

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
			bool is_minimized() const;

			vulkan::command_sequence get_next_frame_context();

			template<typename VertexType>
			vulkan::memory_owning_staging_buffer create_staging_buffer(size_t vertex_count)
			{
				return vulkan::memory_owning_staging_buffer(data->device, sizeof(VertexType) * vertex_count, data->get_device_memory_properties());
			}

			template<typename VertexType>
			vulkan::memory_owning_vertex_buffer create_vertex_buffer(size_t vertex_count)
			{
				return vulkan::memory_owning_vertex_buffer(data->device, sizeof(VertexType) * vertex_count, data->get_device_memory_properties());
			}

			template<typename VertexType>
			vulkan::vertex_array<VertexType> create_vertex_array(size_t vertex_count)
			{
				return vulkan::vertex_array<VertexType>(data->device,vertex_count, data->get_device_memory_properties());
			}

			template<typename VertexType>
			vulkan::vertex_array<VertexType> create_vertex_array(std::initializer_list<VertexType> vertices)
			{
				vulkan::vertex_array<VertexType> result(data->device, vertices.size(), data->get_device_memory_properties());
				std::copy(vertices.begin(), vertices.end(), result.begin());
				return result;
			}
		};
	}
}

#endif