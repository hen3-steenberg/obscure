#ifndef OBSCURE_SHAPE_COLORED_VERTEX_DEFINITION
#define OBSCURE_SHAPE_COLORED_VERTEX_DEFINITION 1
#include "obscure/color/rgba.hpp"
namespace obscure
{
	namespace shape
	{
		struct colored_vertex
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
#endif