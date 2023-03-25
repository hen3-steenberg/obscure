#include "obscure/vulkan/surface.h"

obscure::vulkan::surface::surface() noexcept
	: vk_surface(VK_NULL_HANDLE)
{}


obscure::vulkan::surface::surface(instance instance, glfw_window _window, VkAllocationCallbacks const* allocator)
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.hwnd = _window.get_handle();
	createInfo.hinstance = GetModuleHandle(nullptr);


	VkResult Err = vkCreateWin32SurfaceKHR(instance.get_instance(), &createInfo, allocator, &vk_surface);
	if (Err != VK_SUCCESS) throw Err;
}

void obscure::vulkan::surface::free(instance inst, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroySurfaceKHR(inst.get_instance(), vk_surface, allocator);
}

VkSurfaceKHR obscure::vulkan::surface::get_handle() const noexcept
{
	return vk_surface;
}