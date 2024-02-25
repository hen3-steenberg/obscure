module;
#include "glm/glm.hpp"
export module obscure.shape.colored_vertex;

import obscure.color.rgba;

namespace obscure
{
	namespace shape
	{
		export struct colored_vertex
		{
			glm::fvec3 vertex;
			obscure::color::RGBA color;

			constexpr colored_vertex() noexcept
				: vertex(), color() {}

			constexpr colored_vertex(glm::vec3 _vertex, obscure::color::RGBA _color) noexcept
				: vertex(_vertex), color(_color) {}

			constexpr colored_vertex(glm::vec2 position, obscure::color::RGBA _color) noexcept
				: vertex(position, 1.0f), color(_color) {}
		};
	}
}