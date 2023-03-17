#include "obscure/utilities/name_list.h"

obscure::name_list::name_list(size_t size)
: _size(size), owning(true)
{
	names_ptr = new name[size];
}

obscure::name_list::name_list(name* names, size_t size) noexcept
	: names_ptr(names), _size(size), owning(false)
{}

obscure::name_list::name_list(name_list const& other)
	: _size(other._size), owning(other.owning)
{
	if (!owning)
	{
		names_ptr = other.names_ptr;
	}
	else
	{
		names_ptr = new name[_size];
		for (size_t index = 0; index < _size; ++index)
		{
			names_ptr[index] = other.names_ptr[index];
		}
	}
}

obscure::name_list::name_list(name_list && other) noexcept
	: names_ptr(other.names_ptr), _size(other._size), owning(other.owning)
{
	other.owning = false;
}

obscure::name_list& obscure::name_list::operator=(name_list const& other)
{
	if (owning)
	{
		delete[] names_ptr;
	}

	new (this) name_list(other);
	return *this;
}

obscure::name_list& obscure::name_list::operator=(name_list && other) noexcept
{
	if (owning)
	{
		delete[] names_ptr;
	}

	new (this) name_list(std::move(other));
	return *this;
}

obscure::name_list::~name_list() noexcept
{
	if (owning)
	{
		delete[] names_ptr;
	}
}

obscure::name_list::name obscure::name_list::operator[](size_t index) const
{
	return names_ptr[index];
}

obscure::name_list::name& obscure::name_list::operator[](size_t index)
{
	return names_ptr[index];
}

obscure::name_list::name* obscure::name_list::data() const noexcept
{
	return names_ptr;
}

size_t obscure::name_list::size() const noexcept
{
	return _size;
}

obscure::name_list::operator std::span<obscure::name_list::name>() const noexcept
{
	return std::span<name>(data(), size());
}