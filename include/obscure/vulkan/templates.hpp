#ifndef OBSCURE_VULKAN_TEMPLATE_DEFINITION
#define OBSCURE_VULKAN_TEMPLATE_DEFINITION 1
#include <vector>
#include "obscure/vulkan/glfw_vulkan_include.h"
namespace obscure
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

		template<typename TLoad>
		std::vector<TLoad> vulkan_load(VkResult(__stdcall* enumerator)(uint32_t*, TLoad*))
		{
			uint32_t count;
			enumerator(&count, nullptr);
			std::vector<TLoad> result(count);
			enumerator(&count, result.data());
			return result;
		}

		template<typename TParent, typename TLoad, typename TResult>
		std::vector<TLoad> vulkan_load(TParent p, TResult(__stdcall* enumerator)(TParent, uint32_t*, TLoad*))
		{
			return vulkan_load_data<TLoad, TResult, TParent>(p, enumerator);
		}

		template<typename TParent1, typename TParent2, typename TLoad, typename TResult>
		std::vector<TLoad> vulkan_load(TParent1 p1, TParent2 p2, TResult(__stdcall* enumerator)(TParent1, TParent2, uint32_t*, TLoad*))
		{
			return vulkan_load_data<TLoad, TResult, TParent1, TParent2>(p1, p2, enumerator);
		}


		template<typename TContainer1, typename TContainer2, typename TValue1 = TContainer1::value_type, typename TValue2 = TContainer2::value_type>
		bool contains_all(TContainer1 containing, TContainer2 containees, bool (*is_equal)(TValue1, TValue2))
		{
			for (TValue2 search_item : containees)
			{
				bool found = false;
				for (TValue1 candidate : containing)
				{
					if (is_equal(candidate, search_item))
					{
						found = true;
						break;
					}
				}
				if (!found) return false;
			}
			return true;
		}
	}
}
#endif