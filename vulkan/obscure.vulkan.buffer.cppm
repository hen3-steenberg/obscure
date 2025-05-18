module;
#include <memory>
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
export module obscure.vulkan.buffer;
export import obscure.vulkan.device;

export namespace obscure::vulkan
{

    template<VmaAllocationCreateFlags Flags>
    consteval bool can_write()
    {
        return Flags & (
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
        VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
            );
    }

    template<VmaAllocationCreateFlags Flags>
    consteval bool can_read()
    {
        return Flags & (
        VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT
            );
    }

    template<VmaAllocationCreateFlags Flags>
    consteval bool is_mapped()
    {
        return Flags & VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }

    template<VkBufferUsageFlags Usage>
    consteval bool is_transfer_source()
    {
        return Usage & (VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    }

    template<VkBufferUsageFlags Usage>
    consteval bool is_transfer_destination()
    {
        return Usage & (VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    }

    template<VkBufferUsageFlags Usage, VmaMemoryUsage MemoryUsage, VmaAllocationCreateFlags Flags, size_t Alignment>
    struct buffer_impl : vk::Buffer
    {
        [[nodiscard]] static consteval vk::BufferUsageFlags get_usage_flags() noexcept
        {
            return std::bit_cast<vk::BufferUsageFlags>(Usage);
        }

    private:

        static std::pair<vk::Buffer, VmaAllocation> create_buffer(device const& dev, size_t _size)
        {

            std::pair<vk::Buffer, VmaAllocation> result{VK_NULL_HANDLE, nullptr};
            if(_size == 0) [[unlikely]]
            {
                return result;
            }

            VkBufferCreateInfo create_info = vk::BufferCreateInfo {
                {},
                _size,
                get_usage_flags()
            };

            VmaAllocationCreateInfo alloc_info {};
            alloc_info.usage = MemoryUsage;
            alloc_info.flags = Flags;


            auto * buffer_ptr = reinterpret_cast<VkBuffer*>(&result.first);

            if constexpr (Alignment == 1)
            {

                vmaCreateBuffer(
                    dev.get_vma_allocator(),
                    &create_info,
                    &alloc_info,
                    buffer_ptr,
                    &result.second,
                    nullptr);
            }
            else
            {
                vmaCreateBufferWithAlignment(
                    dev.get_vma_allocator(),
                    &create_info,
                    &alloc_info,
                    Alignment,
                    buffer_ptr,
                    &result.second,
                    nullptr
                    );
            }

            return result;

        }

        buffer_impl(const device& device, std::pair<vk::Buffer, VmaAllocation> buf, size_t size)
            : vk::Buffer(buf.first), allocation(buf.second), vk_device(&device), _size(size)
        {}

    public:

        VmaAllocation allocation{};
        const device * vk_device;
        size_t _size;


        buffer_impl()
            : vk::Buffer(VK_NULL_HANDLE),
        vk_device(),
        _size(0) {}

        buffer_impl(const device& device, size_t size) noexcept
            : buffer_impl(device, create_buffer(device, size), size)
        {

        }

        buffer_impl(buffer_impl const&) = delete;

        buffer_impl(buffer_impl && other) noexcept
            : vk::Buffer(other),
        allocation(other.allocation),
        vk_device(other.vk_device),
        _size(other._size)
        {
            other._size = 0;
        }

        [[nodiscard]] vk::Buffer get_buffer() const noexcept
        {
            return static_cast<vk::Buffer>(*this);
        }

        [[nodiscard]] size_t size() const noexcept
        {
            return _size;
        }

        [[nodiscard]] vk::DeviceSize offset() const noexcept
        {
            VmaAllocationInfo info{};
            vmaGetAllocationInfo(
                vk_device->get_vma_allocator(),
                allocation,
                &info);

            return info.offset;
        }


        template<VmaAllocationCreateFlags Flags2 = Flags>
        requires (can_write<Flags>())
        void copy_data(void const* data, size_t bytes) const
        {
            vmaCopyMemoryToAllocation(
                vk_device->get_vma_allocator(),
                data,
                allocation,
                offset(),
                bytes);
        }

        template<VmaAllocationCreateFlags Flags2 = Flags>
        requires (is_mapped<Flags>())
        [[nodiscard]] void* get_mapped_ptr() const
        {
            VmaAllocationInfo info{};
            vmaGetAllocationInfo(
                vk_device->get_vma_allocator(),
                allocation,
                &info);

            return info.pMappedData;
        }

        ~buffer_impl() noexcept
        {
            if (_size)
            {
                vk_device->waitIdle();
                vmaDestroyBuffer(
                    vk_device->get_vma_allocator(),
                    get_buffer(),
                    allocation);
                _size = 0;
            }
        }

        operator bool() const noexcept
        {
            return _size != 0;
        }
    };

    template<typename T>
    using staging_buffer_impl = buffer_impl<
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
    VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
    std::alignment_of_v<T>>;

    template<typename T>
    struct staging_buffer : public staging_buffer_impl<T>
    {
        size_t _count;
        T* _data;
        staging_buffer(const device& device, size_t count)
            : staging_buffer_impl<T>(device, count * sizeof(T)),
            _count(count),
            _data(std::bit_cast<T*>(staging_buffer_impl<T>::get_mapped_ptr()))
        {}

        T* data() noexcept
        {
            return _data;
        }

        T const* data() const noexcept
        {
            return _data;
        }

        size_t count() const noexcept
        {
            return _count;
        }

        T& operator[](size_t idx) noexcept
        {
            return _data[idx];
        }

        T const& operator[](size_t idx) const noexcept
        {
            return _data[idx];
        }

        T* begin() noexcept
        {
            return _data;
        }

        T const* begin() const noexcept
        {
            return _data;
        }

        T* end() noexcept
        {
            return _data + _count;
        }

        T const* end() const noexcept
        {
            return _data + _count;
        }
    };

    template<typename T>
    using vertex_buffer_impl = buffer_impl<
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    0,
    std::alignment_of_v<T>>;

    template<typename T>
    struct vertex_buffer : vertex_buffer_impl<T>
    {
        size_t _count;

        vertex_buffer(const device& device, size_t count)
            : vertex_buffer_impl<T>(device, count * sizeof(T)),
            _count(count)
        {}

        [[nodiscard]] size_t count() const noexcept
        {
            return _count;
        }
    };

}