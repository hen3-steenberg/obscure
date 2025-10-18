module;
#include <cstdint>
export module obscure.helper_templates.index_of;

namespace obscure::helper_templates
{
    template<std::size_t index, typename Tsearch, typename Ti, typename... Trest>
    struct index_of_impl {
        constexpr static std::size_t value = index_of_impl<index + 1, Tsearch, Trest...>::value;
    };

    template<std::size_t index, typename Tsearch, typename... Trest>
    struct index_of_impl<index, Tsearch, Tsearch, Trest...> {
        constexpr static std::size_t value = index;
    };

    export template<typename Tsearch, typename... Tpack>
    consteval std::size_t
    index_of()
    {
        return index_of_impl<0, Tsearch, Tpack...>::value;
    }
} // namespace obscure::helper_templates