module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.vertex_array;
export import obscure.vulkan.staged_array;

namespace obscure
{
	namespace vulkan
	{
		export template<typename TElem>
		using vertex_array = typename obscure::vulkan::staged_array<TElem, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;
	}
}