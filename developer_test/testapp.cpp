#include "obscure/glfw/glfw_window.hpp"
#include "obscure/version.hpp"
#include "obscure/vulkan/instance.hpp"

int main()
{
	obscure::glfw::glfw_window window{};
	obscure::vulkan::instance instance{"Test App", obscure::version{1,0,0}};

	while (!window.should_close())
	{
		glfwPollEvents();
		if (window.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			break;
		}
	}
	return 0;
}