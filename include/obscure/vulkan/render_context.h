#ifndef OBSCURE_VULKAN_RENDER_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_RENDER_CONTEXT_DEFINITION 1
#include "obscure/vulkan/application_context.h"
#include "obscure/vulkan/owning_buffer.h"
#include "obscure/vulkan/buffer_array.hpp"
#include "obscure/shape/colored_vertex.hpp"

namespace obscure
{
	namespace vulkan
	{
		struct render_context
		{
		protected:
			VkCommandBuffer command_buffer;
			pipeline_collection const& pipelines;
			VkExtent2D extent;
		public:
			render_context(VkCommandBuffer _command_buffer, frame_buffer buffer, application_context const* context);
			render_context& draw_static_triangle();
			render_context& draw_colored_triangle_list(vertex_array<obscure::shape::colored_vertex> & data);
			template<VkIndexType TIndex>
			render_context& draw_colored_triangle_list(vertex_array<obscure::shape::colored_vertex>& vertices, index_array<TIndex>& indices)
			{
				return draw_colored_triangle_list(vertices.device_buffer, indices.device_buffer, TIndex, indices.size());
			}

			render_context& draw_colored_triangle_list(memory_owning_device_buffer& vertex_buffer, memory_owning_device_buffer& index_buffer, VkIndexType type, size_t size);
			render_context& draw_colored_triangle_list(memory_owning_device_buffer& vertex_buffer, size_t size);
			void end_rendering();
		};
	}
}

#endif