#ifndef OBSCURE_VULKAN_REFERENCE_DEFINITION
#define OBSCURE_VULKAN_REFERENCE_DEFINITION 1

#include <memory>
#include <functional>

namespace obscure
{
	namespace vulkan
	{
		template<typename T>
		struct reference : private std::shared_ptr<std::remove_pointer_t<T>>
		{
		protected:
			void set_value(T value, std::function<void(T)> deleter)
			{
				std::shared_ptr<std::remove_pointer_t<T>>::reset(value, deleter);
			}
		public:

			reference() noexcept
				: std::shared_ptr<std::remove_pointer_t<T>>()
			{}

			reference(reference const& other) = default;
			reference(reference&& other) = default;



			T get_handle() const
			{
				return std::shared_ptr<std::remove_pointer_t<T>>::get();
			}
		};
	}
}

#endif