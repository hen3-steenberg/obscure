#ifndef OBSCURE_PARENT_REFERENCE_DEFINITION
#define OBSCURE_PARENT_REFERENCE_DEFINITION 1
#include <cstdint>
#include "obscure/helper_templates/sum.hpp"

namespace obscure
{
    namespace helper_templates
    {
        template<typename TParent, typename ... TSiblings>
        constexpr TParent& get_parent_ref(void* _this)
        {
            static constexpr std::size_t offset = sum<std::size_t, sizeof(TParent), sizeof(TSiblings)...>();
            uint8_t* _parent = reinterpret_cast<uint8_t*>(_this) - offset;
            return *reinterpret_cast<TParent*>(_parent);
        }
    }
}

#endif