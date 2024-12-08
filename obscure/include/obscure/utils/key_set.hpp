#ifndef OBSCURE_KEY_SET_DEFINITION
#define OBSCURE_KEY_SET_DEFINITION 1

#include "obscure/utils/key_list.hpp"

namespace obscure
{
	template<typename Tmap, typename Tlist>
	struct add_list_keys_impl
	{
	};

	template<typename Tmap, template<auto ... > class TList>
	struct add_list_keys_impl<Tmap, TList<>>
	{
		using type = Tmap;
	};

	template<typename Tmap, template<auto ... > class TList, auto Key1, auto ... Keys>
	struct add_list_keys_impl<Tmap, TList<Key1, Keys...>>
	{
		using next_map = typename Tmap::add_key<Key1>;
		using type = std::conditional_t<sizeof...(Keys), typename add_list_keys_impl<next_map, TList<Keys...>>::type, next_map>;
	};

	template<auto ... Keys>
	struct key_set : key_list<Keys...>
	{
		template<auto Key>
		using add_key = std::conditional_t<contains<Key>(), key_set<Keys ...>, key_set<Keys ..., Key>>;

		template<typename TList>
		using add_key_list = typename add_list_keys_impl<key_set<Keys ...>, TList>::type;

		template<auto ... NewKeys>
		using add_keys = add_key_list<key_list<NewKeys...>>;

	};

	template<auto ... Keys>
	using make_set = key_set<>::add_keys<Keys...>;
}

#endif