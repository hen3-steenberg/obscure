module;
#include <filesystem>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.shader_module;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct shader_module
			{
				VkShaderModule vk_module;
				shader_module() noexcept;
				shader_module(device _device, const char* file_name, VkAllocationCallbacks const* allocator = nullptr);
				shader_module(device _device, std::filesystem::path file_path, VkAllocationCallbacks const* allocator = nullptr);

				VkShaderModule get_handle() const noexcept;

				void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;

				VkPipelineShaderStageCreateInfo configure_stage(VkShaderStageFlagBits stage, const char* entry_point = "main") const noexcept;
			};
		}
	}
}