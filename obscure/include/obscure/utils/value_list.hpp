#ifndef OBSCURE_VALUE_LIST_DEFINITION
#define OBSCURE_VALUE_LIST_DEFINITION 1
#include <array>
#include "obscure/utils/key_list.hpp"

namespace obscure
{
	template<typename Tvalue, Tvalue ... values>
	struct value_list : key_list<values...>
	{
		static constexpr std::array<Tvalue, size()> get_values()
		{
			return std::array<Tvalue, size()> { values... };
		}
	};
}

#endif