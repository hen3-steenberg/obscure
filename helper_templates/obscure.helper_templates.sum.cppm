module;
export module obscure.helper_templates.sum;

namespace obscure::helper_templates
{
    template<typename TSum, TSum Arg1, TSum... Args>
    constexpr TSum
    sum_impl()
    {
        if constexpr (sizeof...(Args)) {
            return Arg1 + sum_impl<TSum, Args...>();
        } else {
            return Arg1;
        }
    }

    export template<typename TSum, TSum... Args>
    constexpr TSum
    sum()
    {
        if constexpr (sizeof...(Args)) {
            return sum_impl<TSum, Args...>();
        } else {
            return TSum{};
        }
    }
} // namespace obscure::helper_templates