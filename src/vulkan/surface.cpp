#include "obscure/vulkan/surface.h"

obscure::vulkan::surface::surface(instance instance, window_reference window)
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
	createInfo.hwnd = window.get_handle();
	createInfo.hinstance = GetModuleHandle(nullptr);

	VkSurfaceKHR surface;
	VkResult Err = vkCreateWin32SurfaceKHR(instance.get_instance(), &createInfo, nullptr, &surface);
	if (Err != VK_SUCCESS) throw Err;

	set_value(surface, [instance](VkSurfaceKHR surf)
		{
			vkDestroySurfaceKHR(instance.get_instance(), surf, nullptr);
		});
}