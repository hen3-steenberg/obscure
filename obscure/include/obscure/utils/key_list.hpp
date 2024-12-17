#ifndef OBSCURE_KEY_LIST_DEFINITION
#define OBSCURE_KEY_LIST_DEFINITION 1
#include <type_traits>
#include <tuple>

namespace obscure
{
	template<auto Key, auto Key1, auto ... Keys>
	constexpr bool has_key_impl()
	{
		if constexpr (std::is_same_v<decltype(Key), decltype(Key1)> && Key == Key1)
		{
			return true;
		}
		else if constexpr (sizeof...(Keys))
		{
			return has_key_impl<Key, Keys...>();
		}
		else
		{
			return false;
		}
	}

	template<size_t index, auto Key, auto Key1, auto ... Keys>
	constexpr size_t get_index_impl()
	{
		if constexpr (std::is_same_v<decltype(Key), decltype(Key1)> && Key == Key1)
		{
			return index;
		}
		else if constexpr (sizeof...(Keys))
		{
			return get_index_impl<index + 1, Key, Keys...>();
		}
		else
		{
			static_assert(false, "The key was not found.");
			return -1;
		}
	}

	template<size_t index, auto ... Keys>
	constexpr auto get_at_index_impl()
	{
		auto values = std::make_tuple(Keys...);
		return std::get<index>(values);
	}

	template<auto ... Keys>
	struct key_list
	{

		static constexpr size_t size()
		{
			return sizeof...(Keys);
		}

		template<auto Key>
		static constexpr bool contains()
		{
			if constexpr (size())
			{
				return has_key_impl<Key, Keys...>();
			}
			else
			{
				return false;
			}
		}

		template<auto Key>
		static constexpr size_t get_index()
		{
			static_assert(contains<Key>(), "This list does not contain the key.");
			return get_index_impl<0, Key, Keys...>();
		}

		template<size_t index>
		static constexpr auto get_key()
		{
			static_assert(index < sizeof...(Keys), "The index is out of bounds");
			return get_at_index_impl<index, Keys...>();
		}
	};
}

#endif