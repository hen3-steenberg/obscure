module;
#include <array>
#include <cstdint>
export module obscure.helper_templates.array_initialize;

namespace obscure::helper_templates
{

    template<typename T, typename Fcreate, std::size_t ... Idxs>
    std::array<T, sizeof...(Idxs)> initialize_array_impl(Fcreate creator, std::index_sequence<Idxs...>)
    {
        return std::array<T, sizeof...(Idxs)>{ (Idxs, creator())... };
    }

    export
    template<typename T, std::size_t Size, typename ... TParams>
    std::array<T, Size> initialize_array(TParams ... Params)
    {
        auto create = [&]()
        {
            return T{Params...};
        };
        return initialize_array_impl<T>(create, std::make_index_sequence<Size>{});
    }
}