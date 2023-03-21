#ifndef OBSCURE_VERSION_DEFINITION
#define OBSCURE_VERSION_DEFINITION 1
#include <cstdint>
namespace obscure
{
	struct version
	{
		unsigned int major : 10;
		unsigned int minor : 10;
		unsigned int patch : 12;

		constexpr version() noexcept
			: major(0), minor(0), patch(0) {}

		constexpr version(int _major, int _minor, int _patch) noexcept
			: major(_major), minor(_minor), patch(_patch) {}

		constexpr version(uint32_t packed_version) noexcept
		{
			*this = std::bit_cast<version>(packed_version);
		}

		constexpr uint32_t get_version() const noexcept
		{
			if (std::is_constant_evaluated())
			{
				return (major << 22) | (minor << 12) | patch;
			}
			else
			{
				return std::bit_cast<uint32_t>(*this);
			}
		}

		constexpr explicit operator uint32_t () const noexcept
		{
			return get_version();
		}
	};
}
#endif