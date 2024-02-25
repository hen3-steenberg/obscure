module;
#include <vector>
#include <initializer_list>
export module obscure.vulkan.logger_collection;
import obscure.vulkan.logger;

namespace obscure
{
	namespace vulkan
	{
		export struct logger_collection
		{
		private:
			std::vector<logger*> data;
		public:
			logger_collection() noexcept;
			logger_collection(std::initializer_list<logger*> loggers);
			logger_collection(logger_collection const& other);
			logger_collection(logger_collection&& other) noexcept;

			logger_collection& operator=(logger_collection const& other) = default;

			void free(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr) noexcept;

			void add_logger(logger* logger);
		};
	}
}
