module;
#include <type_traits>
export module obscure.utils.key_set;
export import obscure.utils.key_list;

namespace obscure
{
    template<typename Tmap, typename Tlist>
    struct add_list_keys_impl {
    };

    template<typename Tmap, template<auto...> class TList>
    struct add_list_keys_impl<Tmap, TList<>> {
        using type = Tmap;
    };

    template<typename Tmap, template<auto...> class TList, auto Key1, auto... Keys>
    struct add_list_keys_impl<Tmap, TList<Key1, Keys...>> {
        using next_map = typename Tmap::template add_key<Key1>;
        using type = std::
            conditional_t<sizeof...(Keys) != 0, typename add_list_keys_impl<next_map, TList<Keys...>>::type, next_map>;
    };

    export template<auto... Keys>
    struct key_set {
        using list = key_list<Keys...>;

        static constexpr std::size_t
        size()
        {
            return list::size();
        }

        template<auto Key>
        static constexpr bool
        contains()
        {
            return list::template contains<Key>();
        }

        template<auto Key>
        static constexpr std::size_t
        get_index()
        {
            return list::template get_index<Key>();
        }

        template<std::size_t index>
        static constexpr auto
        get_key()
        {
            return list::template get_key<index>();
        }

        template<auto Key>
        using add_key = std::conditional<contains<Key>(), key_set<Keys...>, key_set<Keys..., Key>>::type;

        template<typename TList>
        using add_key_list = typename add_list_keys_impl<key_set<Keys...>, TList>::type;

        template<auto... NewKeys>
        using add_keys = add_key_list<key_list<NewKeys...>>;
    };

    export template<auto... Keys>
    using make_set = key_set<>::add_keys<Keys...>;
} // namespace obscure
