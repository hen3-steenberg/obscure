#ifndef OBSCURE_SHAPE_OUTLINE_DEFINITION
#define OBSCURE_SHAPE_OUTLINE_DEFINITION 1
namespace obscure
{
	namespace shape
	{
		template<typename T>
		struct outline : public T
		{
			float line_width;
		};
	}
}
#endif