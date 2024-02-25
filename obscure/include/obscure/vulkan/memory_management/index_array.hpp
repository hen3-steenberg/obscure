module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.index_array;
export import obscure.vulkan.staged_array;

namespace obscure
{
	namespace vulkan
	{

		export template<VkIndexType TIndex>
		struct vulkan_index
		{};

		export template<>
		struct vulkan_index<VK_INDEX_TYPE_UINT8_EXT>
		{
			using type = typename uint8_t;
		};

		export template<>
		struct vulkan_index<VK_INDEX_TYPE_UINT16>
		{
			using type = typename uint16_t;
		};

		export template<>
		struct vulkan_index<VK_INDEX_TYPE_UINT32>
		{
			using type = typename uint32_t;
		};

		export template<VkIndexType TIndex>
		using vulkan_index_t = typename vulkan_index<TIndex>::type;

		template<VkIndexType TIndex>
		using index_array = typename obscure::vulkan::staged_array<vulkan_index_t<TIndex>, VK_BUFFER_USAGE_INDEX_BUFFER_BIT>;
	}
}