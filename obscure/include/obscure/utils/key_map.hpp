#ifndef OBSCURE_KEY_MAP_DEFINITION
#define OBSCURE_KEY_MAP_DEFINITION 1
#include "obscure/utils/key_set.hpp"

namespace obscure
{

	template<template<auto> class TValue, auto ... Keys>
	struct key_map : key_set<Keys...>
	{
		std::tuple<TValue<Keys>...> data;

		key_map()
			: data() {
		}

		template<typename ... TParams>
		key_map(TParams ... Parameters)
			: data(std::make_tuple(TValue<Keys> {Parameters...}...)) {}

		template<auto Key>
		TValue<Key> get_value() const&
		{
			return std::get<TValue<Key>>(data);
		}

		template<auto Key>
		TValue<Key>& get_value()&
		{
			return std::get<TValue<Key>>(data);
		}
	};
}
#endif