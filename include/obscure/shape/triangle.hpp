#ifndef OBSCURE_SHAPE_TRIANGLE_DEFINITION
#define OBSCURE_SHAPE_TRIANGLE_DEFINITION 1

#include "obscure/color/rgba.hpp"
#include "outline.hpp"

namespace obscure
{
	namespace shape
	{
		struct triangle
		{
			RGBA color;
			glm::fvec2 points[3];
			float depth;
			typedef outline<triangle> border;
		};
	}
}
#endif