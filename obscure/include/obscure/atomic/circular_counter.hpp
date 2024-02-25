module;
#include <atomic>
#include <compare>
export module obscure.atomic.circular_counter;
import obscure.templates.integer_sizes;

namespace obscure
{
	namespace atomic
	{
		export
		template<size_t maximum_value>
		struct circular_counter
		{
			using counter_type = least_uint_t<maximum_value>;
		private:
			std::atomic<counter_type> counter;
		public:

			constexpr circular_counter()
				: counter(0)
			{}

			counter_type operator++() &
			{
				counter_type result = maximum_value;
				if (counter.compare_exchange_weak(result, 0))
				{
					return 0;
				}
				while (!counter.compare_exchange_weak(result, (++result) % maximum_value)) {}
				return (++result) % maximum_value;
			}

			counter_type operator++(int) &
			{
				counter_type result = maximum_value;
				if (counter.compare_exchange_weak(result, 0))
				{
					return result;
				}
				while (!counter.compare_exchange_weak(result, (++result) % maximum_value)) {}
				return result;
			}

			operator counter_type() const&
			{
				return counter.load();
			}

			inline auto operator<=>(auto other) const&
			{
				return counter.load() <=> other;
			}
		};

		export
		template<>
		struct circular_counter<0>
		{
			using counter_type = void;
			size_t operator++()
			{
				return 0;
			}

			size_t operator++(int)
			{
				return 0;
			}

			operator size_t()
			{
				return 0;
			}

			inline auto operator<=>(auto other)
			{
				return 0 <=> other;
			}
		};
	}
}
