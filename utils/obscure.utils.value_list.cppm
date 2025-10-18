module;
#include <array>
#include <cstdint>
export module obscure.utils.value_list;
export import obscure.utils.key_list;

export namespace obscure
{
    template<typename Tvalue, Tvalue... values>
    struct value_list {
        using list = key_list<values...>;

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

        static constexpr std::array<Tvalue, size()>
        get_values()
        {
            return std::array<Tvalue, size()>{ values... };
        }
    };
} // namespace obscure