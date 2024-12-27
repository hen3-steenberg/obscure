#ifndef OBSCURE_HELPER_TEMPLATE_SUM_DEFINITION
#define OBSCURE_HELPER_TEMPLATE_SUM_DEFINITION 1

namespace obscure
{
    namespace helper_templates
    {
        template<typename TSum,TSum Arg1, TSum ... Args>
        constexpr TSum sum_impl()
        {
            if constexpr (sizeof...(Args))
            {
                return Arg1 + sum_impl<TSum, Args...>();
            }
            else 
            {
                return Arg1;
            }
        }

        template<typename TSum, TSum ... Args>
        constexpr TSum sum()
        {
            if constexpr (sizeof...(Args))
            {
                return sum_impl<TSum, Args...>();
            }
            else 
            {
                return TSum{};
            }
        }
    }
}

#endif