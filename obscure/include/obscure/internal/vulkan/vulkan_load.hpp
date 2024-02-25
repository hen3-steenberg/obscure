module;
#include <vector>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.vulkan_load;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			template<typename TLoad, typename TReturn, typename ... TParent>
			std::vector<TLoad> vulkan_load_data(TParent ... p, TReturn(__stdcall* enumerator)(TParent..., uint32_t*, TLoad*))
			{
				uint32_t count;
				enumerator(p..., &count, nullptr);
				std::vector<TLoad> result(count);
				enumerator(p..., &count, result.data());
				return result;
			}

			export
				template<typename TLoad>
			std::vector<TLoad> vulkan_load(VkResult(__stdcall* enumerator)(uint32_t*, TLoad*))
			{
				uint32_t count;
				enumerator(&count, nullptr);
				std::vector<TLoad> result(count);
				enumerator(&count, result.data());
				return result;
			}

			export
				template<typename TParent, typename TLoad, typename TResult>
			std::vector<TLoad> vulkan_load(TParent p, TResult(__stdcall* enumerator)(TParent, uint32_t*, TLoad*))
			{
				return vulkan_load_data<TLoad, TResult, TParent>(p, enumerator);
			}

			export
				template<typename TParent1, typename TParent2, typename TLoad, typename TResult>
			std::vector<TLoad> vulkan_load(TParent1 p1, TParent2 p2, TResult(__stdcall* enumerator)(TParent1, TParent2, uint32_t*, TLoad*))
			{
				return vulkan_load_data<TLoad, TResult, TParent1, TParent2>(p1, p2, enumerator);
			}
		}
	}
}