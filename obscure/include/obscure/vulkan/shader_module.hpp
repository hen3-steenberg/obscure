#ifndef OBSCURE_VULKAN_SHADER_MODULE_DEFINITION
#define OBSCURE_VULKAN_SHADER_MODULE_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include <initializer_list>
#include <array>

namespace obscure
{
	namespace vulkan
	{

		template<auto shader_key>
		struct shader_module : vk::ShaderModule
		{
		private:


			static vk::ShaderModule create_shader_module(vk::Device device, std::span<const uint32_t> program)
			{
				vk::ShaderModuleCreateInfo create_info
				{
					{},
					program.size_bytes(),
					program.data()
				};

				return device.createShaderModule(create_info);
			}
		public:



			shader_module(vk::Device device)
				: vk::ShaderModule(create_shader_module(device, get_shader_data<shader_key>()))
			{}

			void free(vk::Device device) noexcept
			{
				device.destroyShaderModule(*this);
			}

		};

		

		template<vk::ShaderStageFlagBits Stage>
		using shader_stage_param = vk::ShaderModule;

		template<vk::ShaderStageFlagBits Stage>
		vk::PipelineShaderStageCreateInfo get_builtin_shader_stage_info(shader_stage_param<Stage> shader)
		{
			return vk::PipelineShaderStageCreateInfo
			{
				{},
				Stage,
				shader,
				"main"
			};
		}

		template<vk::ShaderStageFlagBits ... Stages>
		std::array<vk::PipelineShaderStageCreateInfo, sizeof...(Stages)> get_builtin_shader_stage_infos(shader_stage_param<Stages>... shaders)
		{
			return std::array<vk::PipelineShaderStageCreateInfo, sizeof...(Stages)>
			{
				get_builtin_shader_stage_info<Stages>(shaders)...
			};
		}
	}
}

#endif