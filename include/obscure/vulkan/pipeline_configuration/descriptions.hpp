#ifndef OBSCURE_VULKAN_DESCRIPTION_DEFINITIONS
#define OBSCURE_VULKAN_DESCRIPTION_DEFINITIONS 1
#include <array>
#include <tuple>
#include <concepts>
#include "obscure/shape/colored_vertex.hpp"
#include "obscure/vulkan/glfw_vulkan_include.h"

namespace obscure
{
	namespace vulkan
	{
		template<typename ... Bindings>
		constexpr std::array<VkVertexInputBindingDescription, sizeof...(Bindings)> get_input_bining_desciptions(uint32_t instance_bindings = 0)
		{
			constexpr size_t size = sizeof...(Bindings);
			std::array<uint32_t, size> sizes = { sizeof(Bindings)... };
			std::array<VkVertexInputBindingDescription, size> result{};
			for (size_t k = 0; k < size; ++k)
			{
				result[k].binding = k;
				result[k].stride = sizes[k];
				result[k].inputRate = (k < instance_bindings)? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
			}
			
			return result;
		}

		template<typename T>
		constexpr auto get_input_attribute_description(uint32_t binding, uint32_t start_location) = delete;

		template<>
		constexpr auto get_input_attribute_description<obscure::shape::colored_vertex>(uint32_t binding, uint32_t start_location)
		{
			std::array<VkVertexInputAttributeDescription, 2> result{};

			result[0].binding = binding;
			result[0].location = start_location;
			result[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			result[0].offset = offsetof(obscure::shape::colored_vertex, vertex);

			result[1].binding = binding;
			result[1].location = start_location + 1;
			result[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			result[1].offset = offsetof(obscure::shape::colored_vertex, color);

			return result;
		}
		template<size_t first>
		consteval size_t sum()
		{
			return first;
		}

		template<size_t first, size_t ... values>
		consteval size_t sum()
		{
			return first + sum<values...>();
		}

		template<typename T, size_t N0>
		constexpr std::array<T, N0> concatenate_arrays(std::array<T, N0> const& array)
		{
			return array;
		}

		template<typename T, size_t N0, size_t N1>
		constexpr std::array<T, N0 + N1> concatenate_arrays(const std::array<T, N0> array0, const std::array<T, N1> array1)
		{
			std::array<T, N0 + N1> result{};
			size_t index = 0;
			for (T val : array0)
			{
				result[index++] = val;
			}
			for (T val : array1)
			{
				result[index++] = val;
			}
			return result;
		}

		template<typename T, size_t N0, size_t N1, size_t ... N>
		constexpr std::array<T, sum<N0,N1,N...>()> concatenate_arrays(const std::array<T, N0> array0, const std::array<T, N1> array1, const std::array<T, N> ... arrays)
		{
			std::array<T, N0 + N1> result{};
			size_t index = 0;
			for (T val : array0)
			{
				result[index++] = val;
			}
			for (T val : array1)
			{
				result[index++] = val;
			}
			return concatenate_arrays(result, arrays...);
		}

		template<typename Binding>
		constexpr auto get_input_attribute_descriptions_impl(uint32_t binding, uint32_t start_location)
		{
			return get_input_attribute_description<Binding>(binding, start_location);
		}

		template<typename Binding0, typename Binding1>
		constexpr auto get_input_attribute_descriptions_impl(uint32_t binding, uint32_t start_location)
		{
			auto arr1 = get_input_attribute_description<Binding0>(binding, start_location);
			auto arr2 = get_input_attribute_description<Binding1>(binding + 1, start_location + arr1.size());
			return concatenate_arrays(arr1, arr2);
		}

		template<typename Binding0, typename Binding1, typename ... Bindings>
		constexpr auto get_input_attribute_descriptions_impl(uint32_t binding, uint32_t start_location)
		{
			auto arr1 = get_input_attribute_descriptions_impl<Binding0, Binding1>(binding, start_location);
			auto arr2 = get_input_attribute_descriptions_impl<Bindings ...>(binding + 2, start_location + arr1.size());

			return concatenate_arrays(arr1, arr2);
		}

		template<typename ... Bindings>
		constexpr auto get_input_attribute_descriptions()
		{
			return get_input_attribute_descriptions_impl<Bindings...>(0, 0);
		}
	}
}
#endif