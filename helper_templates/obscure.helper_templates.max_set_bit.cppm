module;
#include <bit>
#include <concepts>
#include <type_traits>
export module obscure.helper_templates.max_set_bit;

export namespace obscure::helper_templates
{
    template<typename T>
    concept enum_t = std::is_enum_v<T>;

    template<typename T>
    constexpr auto
    max_set_bits() = delete;

    template<std::unsigned_integral TEnum>
    constexpr auto
    max_set_bits(TEnum Flag)
    {
        return sizeof(TEnum) * 8 - std::countl_zero(Flag);
    }

    template<enum_t TEnum>
    constexpr auto
    max_set_bits(TEnum Flag)
    {
        using uint_t = std::make_unsigned<typename std::underlying_type<TEnum>::type>::type;
        return max_set_bits(std::bit_cast<uint_t>(Flag));
    }
} // namespace obscure::helper_templates