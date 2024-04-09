#include "obscure/glfw/glfw_window.hpp"

int main()
{
	obscure::glfw::glfw_window window{};

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