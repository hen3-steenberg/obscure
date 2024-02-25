export module obscure.internal.contains_all;

namespace obscure
{
	namespace internal
	{
		export
			template<typename TContainer1, typename TContainer2, typename TValue1 = TContainer1::value_type, typename TValue2 = TContainer2::value_type>
		bool contains_all(TContainer1 containing, TContainer2 containees, bool (*is_equal)(TValue1, TValue2))
		{
			for (TValue2 search_item : containees)
			{
				bool found = false;
				for (TValue1 candidate : containing)
				{
					if (is_equal(candidate, search_item))
					{
						found = true;
						break;
					}
				}
				if (!found) return false;
			}
			return true;
		}
	}
}