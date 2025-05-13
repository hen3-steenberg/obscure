module;
#include <memory>
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
export module obscure.vulkan.buffer;
export import obscure.vulkan.device;

export namespace obscure::vulkan
{
    template<VkBufferUsageFlags Usage, size_t Alignment = 1>
    struct buffer : vk::Buffer
    {

        [[nodiscard]] static consteval vk::BufferUsageFlags get_usage_flags() noexcept
        {
            return std::bit_cast<vk::BufferUsageFlags>(Usage);
        }

    private:

        static vk::Buffer create_buffer(device const& dev, size_t _size, VmaAllocation & alloc)
        {

            if(_size == 0) [[unlikely]]
            {
                alloc = nullptr;
                return VK_NULL_HANDLE;
            }

            VkBufferCreateInfo create_info = vk::BufferCreateInfo {
                {},
                _size,
                get_usage_flags()
            };

            VmaAllocationCreateInfo alloc_info {};
            alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

            VkBuffer result{};

            if constexpr (Alignment == 1)
            {

                vmaCreateBuffer(
                    dev.get_vma_allocator(),
                    &create_info,
                    &alloc_info,
                    &result,
                    &alloc,
                    nullptr);
            }
            else
            {
                vmaCreateBufferWithAlignment(
                    dev.get_vma_allocator(),
                    &create_info,
                    &alloc_info,
                    Alignment,
                    &result,
                    &alloc,
                    nullptr
                    );
            }

            return static_cast<vk::Buffer>(result);

        }

    public:

        VmaAllocation allocation{};
        std::reference_wrapper<const device> vk_device;
        size_t _size;



        buffer(const device& device, size_t size) noexcept
            : vk::Buffer(create_buffer(device, size, allocation)),
        vk_device(device),
        _size(size)
        {}

        buffer(buffer const&) = delete;

        buffer(buffer && other) noexcept
            : vk::Buffer(other),
        allocation(other.allocation),
        vk_device(other.vk_device),
        _size(other._size)
        {
            other._size = 0;
        }

        vk::Buffer get_buffer() const noexcept
        {
            return static_cast<vk::Buffer>(*this);
        }

        [[nodiscard]] size_t size() const noexcept
        {
            return _size;
        }

        ~buffer() noexcept
        {
            if (_size)
            {
                vmaDestroyBuffer(
                    vk_device.get().get_vma_allocator(),
                    get_buffer(),
                    allocation);
                _size = 0;
            }
        }
    };

    template<typename T, VkBufferUsageFlags Usage>
    struct write_view
    {
        using buffer_t = buffer<Usage, alignof(T)>;
        std::reference_wrapper<buffer_t> device_buffer;
        std::unique_ptr<T[]> data;


        explicit write_view(buffer_t const& buffer)
            : device_buffer(buffer),
        data(std::make_unique<T[]>(buffer.size() / sizeof(T)))
        {}

        T& operator[](size_t idx) noexcept
        {
            return data[idx];
        }

        T operator[](size_t idx) const noexcept
        {
            return data[idx];
        }

        T* begin() noexcept
        {
            return data;
        }

        T* end() noexcept
        {
            return data + device_buffer.size();
        }

        const T* begin() const noexcept
        {
            return data;
        }

        const T* end() const noexcept
        {
            return data + device_buffer.size();
        }

        ~write_view() noexcept
        {
            if (data)
            {
                VmaAllocationInfo info{};
                vmaGetAllocationInfo(
                    device_buffer.get().get_vma_allocator(),
                    device_buffer.get().allocation,
                    &info);

                vmaCopyMemoryToAllocation(
                    device_buffer.get().get_vma_allocator(),
                    data.get(),
                    device_buffer.get().allocation,
                    info.offset,
                    device_buffer.size());
            }
        }
    };

    template<VkBufferUsageFlags Usage>
    struct write_view<void, Usage>
    {
        using buffer_t = buffer<Usage>;
        std::reference_wrapper<buffer_t> device_buffer;
        std::unique_ptr<void> data;


        explicit write_view(buffer_t const& buffer)
            : device_buffer(buffer),
        data(std::make_unique<void>(buffer.size()))
        {}

        void * get() noexcept
        {
            return data.get();
        }

        const void * get() const noexcept
        {
            return data.get();
        }

        ~write_view() noexcept
        {
            if (data)
            {
                VmaAllocationInfo info{};
                vmaGetAllocationInfo(
                    device_buffer.get().get_vma_allocator(),
                    device_buffer.get().allocation,
                    &info);

                vmaCopyMemoryToAllocation(
                    device_buffer.get().get_vma_allocator(),
                    data.get(),
                    device_buffer.get().allocation,
                    info.offset,
                    device_buffer.size());
            }
        }
    };


}