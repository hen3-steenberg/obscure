module;
#include <cstdint>
#include <bit>
export module version;

export namespace obscure
{
    struct version
    {
        constexpr version() noexcept
            : major(), minor(), patch() {}

        constexpr version(version const& other) noexcept = default;

        constexpr explicit version(uint32_t vers) noexcept
        {
            *this = std::bit_cast<version, uint32_t>(vers);
        }

        constexpr version(uint32_t Major, uint32_t Minor, uint32_t Patch) noexcept
            : major(Major), minor(Minor), patch(Patch)
        {}

        uint32_t patch : 12;
        uint32_t minor : 10;
        uint32_t major : 10;

        [[nodiscard]] constexpr  uint32_t vulkan_version() const noexcept
        {
            return std::bit_cast<uint32_t, version>(*this);
        }

        [[nodiscard]] constexpr explicit operator uint32_t () const noexcept
        {
            return vulkan_version();
        }
    };
}