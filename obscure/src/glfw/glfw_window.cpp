#include "obscure/glfw/glfw_defaults.hpp"
#include "obscure/glfw/glfw_window.hpp"
#include "obscure/obscure_properties.hpp"

obscure::glfw::glfw_window::glfw_window()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
	if constexpr (enable_debug_validation())
	{
		window_ptr = glfwCreateWindow(get_default_window_width(), get_default_window_height(), "", nullptr, nullptr);
	}
	else
	{
		window_ptr = glfwCreateWindow(get_default_window_width(), get_default_window_height(), "", glfwGetPrimaryMonitor(), nullptr);
	}
	
}

obscure::glfw::glfw_window::glfw_window(glfw_window&& other)
	: glfw_window_ref(other)
{
	other.window_ptr = nullptr;
}

[[nodiscard]] obscure::glfw::glfw_window_ref obscure::glfw::glfw_window::get_window_ref() const noexcept {
	return static_cast<obscure::glfw::glfw_window_ref>(*this);
}

obscure::glfw::glfw_window::~glfw_window()
{
	if (window_ptr)
	{
		glfwDestroyWindow(window_ptr);
	}
}