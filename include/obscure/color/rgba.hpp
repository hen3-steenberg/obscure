#ifndef OBSCURE_COLOR_RGBA_DEFINITION
#define OBSCURE_COLOR_RGBA_DEFINITION 1
#include "glm/glm.hpp"

namespace obscure
{
	namespace color
	{
		struct RGBA : public glm::fvec4
		{
			constexpr RGBA()
				: glm::fvec4()
			{}

			constexpr RGBA(float r, float g, float b, float a = 1.0f)
				: glm::fvec4(r, g, b, a)
			{}

			constexpr RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
				: glm::fvec4(r / 256.0f, g / 256.0f, b / 256.0f, a / 256.0f)
			{}

			constexpr RGBA(uint32_t rgba)
				: glm::fvec4(((0xff000000 & rgba) >> 24) / 256.0f, ((0x00ff0000 & rgba) >> 16) / 256.0f, ((0x0000ff00 & rgba) >> 8) / 256.0f, (0x000000ff & rgba) / 256.0f)
			{}

			constexpr RGBA(const RGBA & other)
				: glm::fvec4(other)
			{}
		};

		namespace RGB
		{
			constexpr const RGBA RED = RGBA(1.0f, 0.0f, 0.0f);
			constexpr const RGBA GREEN = RGBA(0.0f, 1.0f, 0.0f);
			constexpr const RGBA BLUE = RGBA(0.0f, 0.0f, 1.0f);

		};
	}
}
#endif