module;
#include <initializer_list>
#include <utility>
module obscure.vulkan.logger_collection;

obscure::vulkan::logger_collection::logger_collection() noexcept
	: data()
{}

obscure::vulkan::logger_collection::logger_collection(std::initializer_list<logger*> loggers)
	: data(loggers)
{}

obscure::vulkan::logger_collection::logger_collection(logger_collection const& other)
	: data(other.data)
{}

obscure::vulkan::logger_collection::logger_collection(logger_collection&& other) noexcept
	: data(std::move(other.data))
{}

void obscure::vulkan::logger_collection::free(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator) noexcept
{
	for (logger* log : data)
	{
		log->free(instance, allocator);
		delete log;
	}
}

void obscure::vulkan::logger_collection::add_logger(logger* logger)
{
	data.push_back(logger);
}