#ifndef OBSCURE_VULKAN_OWNING_BUFFER_DEFINITION
#define OBSCURE_VULKAN_OWNING_BUFFER_DEFINITION 1

#include "obscure/vulkan/device_memory.h"
#include "obscure/vulkan/buffer_memory.h"
#include <span>
#include <atomic>

namespace obscure
{
	namespace vulkan
	{
		struct memory_owning_buffer
		{
			VkBuffer vk_buffer;
			device_memory memory;
			device device;
			const VkAllocationCallbacks* allocator;
			std::atomic_uint64_t* counter;

			memory_owning_buffer() noexcept;
			memory_owning_buffer(vulkan::device _device, size_t size, VkBufferUsageFlags usage, VkPhysicalDeviceMemoryProperties properties, VkMemoryPropertyFlags requested_properties, const VkAllocationCallbacks* _allocator = nullptr);
			memory_owning_buffer(memory_owning_buffer const& other) noexcept;
			memory_owning_buffer(memory_owning_buffer && other) noexcept;

			memory_owning_buffer& operator=(memory_owning_buffer const& other) noexcept;
			memory_owning_buffer& operator=(memory_owning_buffer && other) noexcept;

			VkBuffer get_handle() const& noexcept;
			bool valid() const&;

			virtual ~memory_owning_buffer();
		};

		struct memory_owning_mapped_staging_buffer : public memory_owning_buffer
		{
			void* mapped_memory;

			void sync_memory() &;

			memory_owning_mapped_staging_buffer() noexcept;
			memory_owning_mapped_staging_buffer(vulkan::device _device, size_t buffer_size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr);
			memory_owning_mapped_staging_buffer(memory_owning_mapped_staging_buffer const& other) = default;
			memory_owning_mapped_staging_buffer(memory_owning_mapped_staging_buffer && other) = default;
			memory_owning_mapped_staging_buffer& operator=(memory_owning_mapped_staging_buffer const& other) = default;
			memory_owning_mapped_staging_buffer& operator=(memory_owning_mapped_staging_buffer && other) = default;
			~memory_owning_mapped_staging_buffer();
		};

		struct memory_owning_staging_buffer : public memory_owning_buffer
		{
			memory_owning_staging_buffer() noexcept = default;
			memory_owning_staging_buffer(vulkan::device _device, size_t buffer_size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr);
			memory_owning_staging_buffer(memory_owning_staging_buffer const& other) noexcept = default;
			memory_owning_staging_buffer(memory_owning_staging_buffer && other) noexcept = default;

			memory_owning_staging_buffer& operator=(memory_owning_staging_buffer const& other) noexcept = default;
			memory_owning_staging_buffer& operator=(memory_owning_staging_buffer && other) noexcept = default;

			void write_data(const void* data, size_t size)&;

			buffer_memory map_memory(size_t size)&;

			template<typename T>
			void write_data(std::span<T> data)&
			{
				write_data(data.data(), sizeof(T) * data.size());
			}
		};

		struct memory_owning_device_buffer : public memory_owning_buffer
		{
			memory_owning_device_buffer() noexcept = default;
			memory_owning_device_buffer(vulkan::device _device, size_t buffer_size, VkBufferUsageFlags usage, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr);
			memory_owning_device_buffer(memory_owning_device_buffer const& other) noexcept = default;
			memory_owning_device_buffer(memory_owning_device_buffer && other) noexcept = default;
			memory_owning_device_buffer& operator=(memory_owning_device_buffer const& other) noexcept = default;
			memory_owning_device_buffer& operator=(memory_owning_device_buffer && other) noexcept = default;
		};
	}
}
#endif