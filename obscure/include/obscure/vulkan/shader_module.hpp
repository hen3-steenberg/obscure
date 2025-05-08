#ifndef OBSCURE_VULKAN_SHADER_MODULE_DEFINITION
#define OBSCURE_VULKAN_SHADER_MODULE_DEFINITION 1
#include "../../../../modules/glfw_vulkan_include.hpp"
#include <initializer_list>
#include <array>

namespace obscure
{
	namespace vulkan
	{
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
			explicit shader_module(vk::Device device, std::span<const uint32_t> program)
				: vk::ShaderModule(create_shader_module(device, program))
			{}

			template<auto Key>
			static shader_module load_shader(vk::Device device) {
				return shader_module{ device, get_shader_data<Key>() };
			}

			[[nodiscard]] vk::ShaderModule get() const&
			{
				return *this;
			}

			void free(vk::Device device) noexcept
			{
				device.destroyShaderModule(get());
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