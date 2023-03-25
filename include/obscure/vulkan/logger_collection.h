#ifndef OBSCURE_VULKAN_LOGGER_COLLECTION_DEFINITION
#define OBSCURE_VULKAN_LOGGER_COLLECTION_DEFINITION 1
#include <vector>
#include <initializer_list>
#include "obscure/vulkan/logger.h"
namespace obscure
{
	namespace vulkan
	{
		struct logger_collection
		{
		private:
			std::vector<logger*> data;
		public:
			logger_collection() noexcept;
			logger_collection(std::initializer_list<logger*> loggers);
			logger_collection(logger_collection const& other);
			logger_collection(logger_collection&& other) noexcept;

			logger_collection& operator=(logger_collection const& other) = default;

			void free(instance instance, VkAllocationCallbacks const* allocator = nullptr) noexcept;

			void add_logger(logger* logger);
		};
	}
}

#endif