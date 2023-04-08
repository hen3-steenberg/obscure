#include "obscure/vulkan/shader_module.h"
#include "obscure/utilities/read_file.h"

obscure::vulkan::shader_module::shader_module() noexcept
	: vk_module(VK_NULL_HANDLE)
{}

obscure::vulkan::shader_module::shader_module(device _device, const char* file_name, VkAllocationCallbacks const* allocator)
{
	std::vector<char> code = obscure::read_file(file_name);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;

	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());


	vkCreateShaderModule(_device.get_handle(), &createInfo, allocator, &vk_module);
}


obscure::vulkan::shader_module::shader_module(device _device, std::filesystem::path file_path, VkAllocationCallbacks const* allocator)
{
	std::vector<char> code = obscure::read_file(file_path);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;

	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());


	vkCreateShaderModule(_device.get_handle(), &createInfo, allocator, &vk_module);
}

void obscure::vulkan::shader_module::free(device _device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyShaderModule(_device.get_handle(), vk_module, nullptr);
}

VkPipelineShaderStageCreateInfo obscure::vulkan::shader_module::configure_stage(VkShaderStageFlagBits stage, const char* entry_point) const noexcept
{
	VkPipelineShaderStageCreateInfo result{};
	result.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	result.pNext = nullptr;

	result.stage = stage;

	result.module = get_handle();

	result.pName = entry_point;

	return result;
}

VkShaderModule obscure::vulkan::shader_module::get_handle() const noexcept
{
	return vk_module;
}