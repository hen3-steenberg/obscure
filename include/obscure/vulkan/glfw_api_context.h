#ifndef OBSCURE_GLFW_API_CONTEXT_DEFINITION
#define OBSCURE_GLFW_API_CONTEXT_DEFINITION 1
#include <span>
namespace obscure
{
	namespace vulkan
	{
		struct glfw_api_contex
		{
			glfw_api_contex();
			~glfw_api_contex();
			static std::span<const char*> GetRequiredInstanceExtensions();
		};
	}
}
#endif