module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.surface;

obscure::internal::vulkan::surface::surface() noexcept
	: vk_surface(VK_NULL_HANDLE)
{}


obscure::internal::vulkan::surface::surface(instance instance, glfw_window _window, VkAllocationCallbacks const* allocator)
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.hwnd = _window.get_handle();
	createInfo.hinstance = GetModuleHandle(nullptr);


	VkResult Err = vkCreateWin32SurfaceKHR(instance.get_instance(), &createInfo, allocator, &vk_surface);
	if (Err != VK_SUCCESS) throw Err;
}

void obscure::internal::vulkan::surface::free(instance inst, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroySurfaceKHR(inst.get_instance(), vk_surface, allocator);
}

VkSurfaceKHR obscure::internal::vulkan::surface::get_handle() const noexcept
{
	return vk_surface;
}