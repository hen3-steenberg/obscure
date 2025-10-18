module;
#include <vector>
export module obscure.helper_templates.vector_initialize;

namespace obscure::helper_templates
{
    export template<typename T, typename... TParams>
    std::vector<T>
    initialize_vector(std::size_t size, TParams... Params)
    {
        std::vector<T> result;
        result.reserve(size);
        for (std::size_t idx = 0; idx < size; ++idx) {
            result.emplace_back(Params...);
        }
        return result;
    }
} // namespace obscure::helper_templates