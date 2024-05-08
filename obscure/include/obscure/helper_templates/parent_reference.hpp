#ifndef OBSCURE_PARENT_REFERENCE_DEFINITION
#define OBSCURE_PARENT_REFERENCE_DEFINITION 1
#include <cstdint>
#include "obscure/helper_templates/sum.hpp"

namespace obscure
{
    namespace helper_templates
    {
        template<typename TParent, typename ... TSiblings>
        struct parent_ref
        {
            static constexpr std::size_t offset = sum<std::size_t, sizeof(TParent), sizeof(TSiblings)...>();

            const TParent & get_parent_cref() const&
            {
                uint8_t const* _this = reinterpret_cast<uint8_t const*>(this);
                uint8_t const* _parent = _this - offset;
                return *reinterpret_cast<TParent const*>(_parent);
            }

            const TParent & get_parent_ref() const&
            {
                uint8_t const* _this = reinterpret_cast<uint8_t const*>(this);
                uint8_t const* _parent = _this - offset;
                return *reinterpret_cast<TParent const*>(_parent);
            }

            TParent & get_parent_ref() &
            {
                uint8_t* _this = reinterpret_cast<uint8_t*>(this);
                uint8_t* _parent = _this - offset;
                return *reinterpret_cast<TParent*>(_parent);
            }
        };
    }
}

#endif