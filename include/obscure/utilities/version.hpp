#ifndef OBSCURE_VERSION_DEFINITION
#define OBSCURE_VERSION_DEFINITION 1
namespace obscure
{
	struct version
	{
		unsigned int major : 10;
		unsigned int minor : 10;
		unsigned int patch : 12;
		constexpr unsigned long get_vk_version() const noexcept
		{
			if (std::is_constant_evaluated())
			{
				return (major << 22) | (minor << 12) | patch;
			}
			else
			{
				return *reinterpret_cast<const unsigned long*>(this);
			}
		}
	};
}
#endif