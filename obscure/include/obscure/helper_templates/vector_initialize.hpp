#ifndef OBSCURE_HELPER_TEMPLATES_VECTOR_INITIALIZE_DEFINITION
#define OBSCURE_HELPER_TEMPLATES_VECTOR_INITIALIZE_DEFINITION
#include <vector>

namespace obscure
{
    namespace helper_templates
    {
        template<typename T, typename ... TParams>
        std::vector<T> initialize_vector(std::size_t size, TParams ... Params)
        {
            std::vector<T> result;
            result.reserve(size);
            for(std::size_t idx = 0; idx < size; ++idx)
            {
                result.emplace_back(Params...);
            }
            return result;
        }
    }
}
#endif