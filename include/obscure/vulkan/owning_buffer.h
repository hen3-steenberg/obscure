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

			memory_owning_buffer& operator=(memory_owning_buffer const& other) noexcept;

			template<typename T>
			static memory_owning_buffer create_vertex_buffer(vulkan::device _device, size_t vertex_count, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr)
			{
				return memory_owning_buffer(_device, sizeof(T) * vertex_count, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, properties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, allocator);
			}

			VkBuffer get_handle() const& noexcept;
			

			~memory_owning_buffer();
		};

		struct memory_owning_staging_buffer : public memory_owning_buffer
		{
			memory_owning_staging_buffer() noexcept = default;
			memory_owning_staging_buffer(vulkan::device _device, size_t buffer_size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr);
			memory_owning_staging_buffer(memory_owning_staging_buffer const& other) noexcept = default;

			memory_owning_staging_buffer& operator=(memory_owning_staging_buffer const& other) noexcept = default;

			void write_data(const void* data, size_t size)&;

			buffer_memory map_memory(size_t size)&;

			template<typename T>
			void write_data(std::span<T> data)&
			{
				write_data(data.data(), sizeof(T) * data.size());
			}
		};

		struct memory_owning_vertex_buffer : public memory_owning_buffer
		{
			memory_owning_vertex_buffer() noexcept = default;
			memory_owning_vertex_buffer(vulkan::device _device, size_t buffer_size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr);
			memory_owning_vertex_buffer(memory_owning_vertex_buffer const& other) noexcept = default;
			memory_owning_vertex_buffer& operator=(memory_owning_vertex_buffer const& other) noexcept = default;
		};
	}
}
#endif