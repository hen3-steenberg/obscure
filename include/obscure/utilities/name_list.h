#ifndef OBSCURE_NAME_LIST_DEFINITION
#define OBSCURE_NAME_LIST_DEFINITION 1
#include <span>
namespace obscure
{
	struct name_list
	{
		typedef const char* name;
	private:
		name* names_ptr;
		size_t _size;
		bool owning;
	public:

		name_list(size_t size);

		name_list(name* names, size_t size) noexcept;

		name_list(name_list const& other);

		name_list(name_list&& other) noexcept;

		name_list& operator=(name_list const& other);

		name_list& operator=(name_list&& other) noexcept;

		~name_list() noexcept;

		name operator[](size_t index) const;

		name& operator[](size_t index);

		name* data() const noexcept;

		size_t size() const noexcept;

		operator std::span<name>() const noexcept;

	};
}

#endif