#ifndef OBSCURE_PARENT_REFERENCE_DEFINITION
#define OBSCURE_PARENT_REFERENCE_DEFINITION 1
#include <cstdint>
#include <tuple>
#include "obscure/helper_templates/sum.hpp"

namespace obscure
{
    namespace helper_templates
    {

        template<typename ... Types>
        struct layout_impl;

        template<typename T1, typename T2, typename ... Types>
        struct layout_impl<T1, T2, Types...>
        {
            T1 _field;
            layout_impl<T2, Types...> _fields;
        };

        template<typename T1>
        struct layout_impl<T1>
        {
            T1 field;
        };

        template<typename ... Types>
        struct layout_definition
        {
            layout_impl<Types...> _fields;
            void* last;
        };

        template<typename ... Types>
        constexpr size_t get_offset()
        {
            using layout = layout_definition<Types...>;
            return offsetof(layout, last);
        }

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
            static constexpr std::size_t offset = get_offset<TParent, TSiblings...>();
            return get_parent_ref_impl<TParent, offset>(_this);
        }
    }
}

#endif