module;
#include <span>
#include <unordered_map>
#include <filesystem>
#include <type_traits>
#include "glfw_vulkan_include.h"
export module obscure.vulkan.graphics_pipeline_configuration;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.swap_chain;
import obscure.internal.vulkan.shader_module;
import obscure.internal.vulkan.pipeline_layout;
import obscure.internal.vulkan.render_pass;

namespace obscure
{
	namespace vulkan
	{

		export struct graphics_pipeline_configuration
		{
		protected:
			obscure::internal::vulkan::device device;
			obscure::internal::vulkan::swap_chain swap_chain;
			VkAllocationCallbacks const* allocator;
			graphics_pipeline_configuration(obscure::internal::vulkan::device _device, obscure::internal::vulkan::swap_chain _swap_chain, VkAllocationCallbacks const* _allocator);
			virtual std::span<VkPipelineShaderStageCreateInfo> shader_stages(std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module> const& loaded_shaders) = 0;
			virtual VkPipelineDynamicStateCreateInfo* dynamic_state() = 0;
			virtual VkPipelineVertexInputStateCreateInfo* vertex_input_state() = 0;
			virtual VkPipelineInputAssemblyStateCreateInfo* input_assembly_state() = 0;
			virtual VkPipelineViewportStateCreateInfo* viewport_state() = 0;
			virtual VkPipelineMultisampleStateCreateInfo* multisample_state() = 0;
			virtual VkPipelineRasterizationStateCreateInfo* rasterization_state() = 0;
			virtual VkPipelineColorBlendStateCreateInfo* color_blend_state() = 0;
			virtual VkPipelineDepthStencilStateCreateInfo* depth_stencil_state() = 0;
		public:
			virtual std::span<std::filesystem::path> shader_stage_paths() = 0;
			virtual obscure::internal::vulkan::pipeline_layout layout() = 0;
			VkGraphicsPipelineCreateInfo parse_configuration(std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module> const& loaded_shaders);
		};

		export
		template<typename T>
		concept pipeline_configuration = std::is_base_of_v<graphics_pipeline_configuration, T> && !std:: is_abstract_v<T>;
	}
}