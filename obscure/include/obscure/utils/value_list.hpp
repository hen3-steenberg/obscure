#ifndef OBSCURE_VALUE_LIST_DEFINITION
#define OBSCURE_VALUE_LIST_DEFINITION 1
#include <array>
#include "obscure/utils/key_list.hpp"

namespace obscure
{
	template<typename Tvalue, Tvalue ... values>
	struct value_list
	{
		using list = key_list<values...>;

		static constexpr size_t size()
		{
			return list::size();
		}

		template<auto Key>
		static constexpr bool contains()
		{
			return list:: template contains<Key>();
		}

		template<auto Key>
		static constexpr size_t get_index()
		{
			return list:: template get_index<Key>();
		}

		template<size_t index>
		static constexpr auto get_key()
		{
			return list:: template get_key<index>();
		}

		static constexpr std::array<Tvalue, size()> get_values()
		{
			return std::array<Tvalue, size()> { values... };
		}
	};
}

#endif
