module;
#include <span>
export module obscure.internal.vulkan.glfw_api_context;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct glfw_api_contex
			{
				glfw_api_contex();
				~glfw_api_contex();
				static std::span<const char*> GetRequiredInstanceExtensions();
			};
		}
	}
}