#ifndef OBSCURE_PARENT_REFERENCE_DEFINITION
#define OBSCURE_PARENT_REFERENCE_DEFINITION 1
#include <cstdint>
#include <tuple>
#include "obscure/helper_templates/sum.hpp"

namespace obscure
{
    namespace helper_templates
    {

        template<typename T>
        using raw_ptr_t = std::conditional_t<std::is_const_v<T>, const void *, void *>;

        template<typename TParent, size_t offset>
        constexpr TParent& get_parent_ref_impl(raw_ptr_t<TParent> _this)
        {
            using byte_ptr_t = std::conditional_t<std::is_const_v<TParent>, const uint8_t *, uint8_t *>;
            byte_ptr_t _parent = reinterpret_cast<byte_ptr_t>(_this) - offset;
            return *reinterpret_cast<TParent*>(_parent);
        }

        template<typename TParent, typename ... TSiblings>
        constexpr TParent& get_parent_ref(raw_ptr_t<TParent> _this)
        {
            static constexpr std::size_t offset = sizeof(std::tuple<TParent, TSiblings...>);
            return get_parent_ref_impl<TParent, offset>(_this);
        }
    }
}

#endif