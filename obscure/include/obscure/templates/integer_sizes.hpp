module;
#include <concepts>
#include <limits>
export module obscure.templates.integer_sizes;

export 
template<std::integral T, size_t max_size>
constexpr bool is_large_enough = std::numeric_limits<T>::max() >= max_size;

export
template<std::integral T>
struct next_size_integer
{};

export
template<>
struct next_size_integer<uint8_t>
{
	using type = uint16_t;
};

export
template<>
struct next_size_integer<uint16_t>
{
	using type = uint32_t;
};

export
template<>
struct next_size_integer<uint32_t>
{
	using type = uint64_t;
};

export
template<>
struct next_size_integer<int8_t>
{
	using type = int16_t;
};

export
template<>
struct next_size_integer<int16_t>
{
	using type = int32_t;
};

export
template<>
struct next_size_integer<int32_t>
{
	using type = int64_t;
};

export
template<std::integral T>
using next_size_integer_t = next_size_integer<T>::type;


template<std::integral T, size_t max_size>
struct least_int_impl
{
	using next_type = least_int_impl<next_size_integer_t<T>, max_size>;
	using type = std::conditional_t<is_large_enough<T, max_size>, T,typename next_type::type>;
};

template<size_t max_size>
struct least_int_impl<uint64_t, max_size>
{
	using type = std::enable_if_t<is_large_enough<uint64_t, max_size>, uint64_t>;
};

template<size_t max_size>
struct least_int_impl<int64_t, max_size>
{
	using type = std::enable_if_t<is_large_enough<int64_t, max_size>, int64_t>;
};

export template<size_t max_size>
using least_int = least_int_impl<int8_t, max_size>;

export template<size_t max_size>
using least_int_t = least_int<max_size>::type;

export template<size_t max_size>
using least_uint = least_int_impl<uint8_t, max_size>;

export template<size_t max_size>
using least_uint_t = least_uint<max_size>::type;

