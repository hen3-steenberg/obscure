module;
#include <vector>
#include <cstdint>
export module obscure.utils.name_list;

export namespace obscure
{
    template<bool OWNING>
    struct name_list : std::vector<const char*>
    {
        name_list(const char** names, uint32_t name_count)
            : std::vector<const char*>(names, names + name_count)
        {}

        uint32_t get_count() const noexcept
        {
            return static_cast<uint32_t>(size());
        }

        const char** get_names() const noexcept
        {
            return const_cast<const char**>(data());
        }
    };

    template<>
    struct name_list<false>
    {
        const char** names;
        uint32_t name_count;
        name_list(const char** _names, uint32_t _name_count) noexcept
            : names(_names), name_count(_name_count)
        {}

        uint32_t get_count() const noexcept
        {
            return name_count;
        }

        const char** get_names() const noexcept
        {
            return names;
        }

        void push_back(const char*) const noexcept
        {}
    };
}