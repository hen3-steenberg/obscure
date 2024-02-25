module;
#include <filesystem>
#include <unordered_map>
#include <span>
#include <array>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.initialize_pipeline_collection;
import obscure.internal.vulkan.pipeline_layout;
import obscure.internal.vulkan.pipeline_collection;
import obscure.internal.vulkan.shader_module;
import obscure.internal.vulkan.swap_chain;
import obscure.vulkan.graphics_pipeline_configuration;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
#pragma region pipeline_layout_initialization
			template<size_t N, size_t index, obscure::vulkan::pipeline_configuration Tconfig, obscure::vulkan::pipeline_configuration ... Tconfigs>
			void initialize_pipeline_layouts(std::array<obscure::internal::vulkan::pipeline_layout, N> & result, Tconfig & config, Tconfigs & ... configs)
			{
				 result[index] = config.layout();
				 initialize_pipeline_layout<N, index + 1, Tconfigs...>(result, configs ...);
			}

			template<size_t N, size_t index, obscure::vulkan::pipeline_configuration Tconfig>
			void initialize_pipeline_layouts(std::array<obscure::internal::vulkan::pipeline_layout, N>& result, Tconfig & config)
			{
				result[index] = config.layout();
			}
#pragma endregion

#pragma region shader_loading

			template<obscure::vulkan::pipeline_configuration Tconfig, obscure::vulkan::pipeline_configuration ... Tconfigs>
			void initialize_shaders(std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module>& loaded_shaders, obscure::internal::vulkan::device device, const VkAllocationCallbacks* allocator, Tconfig & config, Tconfigs &... configs)
			{
				std::span < std::filesystem::path> shader_paths = config.shader_stage_paths();
				for (auto const& path : shader_paths)
				{
					if (!loaded_shaders.contains(path))
					{
						loaded_shaders[path] = obscure::internal::vulkan::shader_module(device, path, allocator);
					}
				}
				initialize_shaders<Tconfigs...>(loaded_shaders, device, allocator, configs...);
			}

			template<obscure::vulkan::pipeline_configuration Tconfig>
			void initialize_shaders(std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module>& loaded_shaders, obscure::internal::vulkan::device device, const VkAllocationCallbacks* allocator, Tconfig & config)
			{
				std::span < std::filesystem::path> shader_paths = config.shader_stage_paths();
				for (auto const& path : shader_paths)
				{
					if (!loaded_shaders.contains(path))
					{
						loaded_shaders[path] = obscure::internal::vulkan::shader_module(device, path, allocator);
					}
				}
			}
#pragma endregion

#pragma region pipeline_create_info
			template<size_t N, size_t index, obscure::vulkan::pipeline_configuration Tconfig, obscure::vulkan::pipeline_configuration ... Tconfigs>
			void get_pipeline_create_infos(std::array<VkGraphicsPipelineCreateInfo, N>& result, std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module> const& loaded_shaders, Tconfig & config, Tconfigs & ... configs)
			{
				result[index] = config.parse_configuration(loaded_shaders);
				initialize_pipeline_layout<N, index + 1, Tconfigs...>(result, loaded_shaders, configs ...);
			}

			template<size_t N, size_t index, obscure::vulkan::pipeline_configuration Tconfig>
			void get_pipeline_create_infos(std::array<VkGraphicsPipelineCreateInfo, N>& result, std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module> const& loaded_shaders, Tconfig & config)
			{
				result[index] = config.parse_configuration(loaded_shaders);
			}
#pragma endregion
			template<obscure::vulkan::pipeline_configuration ... Tconfigs>
			pipeline_collection<sizeof...(Tconfigs)> initialize_pipeline_collection(obscure::internal::vulkan::device device, const VkAllocationCallbacks* allocator, Tconfigs ... configs)
			{
				constexpr size_t pipeline_count = sizeof...(Tconfigs);
				pipeline_collection<pipeline_count> result;

				initialize_pipeline_layout<pipeline_count, 0, Tconfigs ...>(result.layout_data, configs...);


				std::unordered_map<std::filesystem::path, obscure::internal::vulkan::shader_module> loaded_shaders;
				initialize_shaders<Tconfigs...>(loaded_shaders, device, allocator, configs...);

				result.shader_modules.reserve(loaded_shaders.size());

				for (auto const& shader : loaded_shaders)
				{
					result.shader_modules.push_back(shader.second);
				}

				std::array<VkGraphicsPipelineCreateInfo, pipeline_count> pipeline_create_infos;
				get_pipeline_create_infos<pipeline_count, 0, Tconfigs...>(pipeline_create_infos, loaded_shaders, configs...);

				vkCreateGraphicsPipelines(device.get_handle(), VK_NULL_HANDLE, static_cast<uint32_t>(pipeline_count), pipeline_create_infos.data(), allocator, result.pipeline_data.data());

				return result;
			}

			export template<obscure::vulkan::pipeline_configuration ... Tconfigs>
			pipeline_collection<sizeof...(Tconfigs)> initialize_pipeline_collection(obscure::internal::vulkan::device device, obscure::internal::vulkan::swap_chain swap_chain, const VkAllocationCallbacks* allocator)
			{
				return initialize_pipeline_collection<Tconfigs...>(device, allocator, Tconfigs(device, swap_chain, allocator)...);
			}
		}
	}
}