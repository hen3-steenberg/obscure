module;
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.command_pool;

export import obscure.vulkan.device;

export namespace obscure::vulkan
{
    struct command_pool
        : vk::CommandPool
        , private vk::Device {
    private:
        [[nodiscard]] vk::Device
        get_device() const noexcept
        {
            return static_cast<vk::Device>(*this);
        }

        static vk::CommandPool
        create_command_pool(device const& _device)
        {
            vk::CommandPoolCreateInfo create_info{ vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                   _device.graphics_queue_index };

            return _device.createCommandPool(create_info);
        }

    public:
        std::vector<vk::CommandBuffer> command_buffers;

        explicit command_pool(device const& _device, std::size_t buffer_count)
            : vk::CommandPool(create_command_pool(_device))
            , vk::Device(_device.get_device())
        {
            vk::CommandBufferAllocateInfo create_info{ get_command_pool(),
                                                       vk::CommandBufferLevel::ePrimary,
                                                       static_cast<uint32_t>(buffer_count) };

            command_buffers = get_device().allocateCommandBuffers(create_info);
        }

        [[nodiscard]] vk::CommandPool
        get_command_pool() const&
        {
            return static_cast<vk::CommandPool>(*this);
        }

        [[nodiscard]] vk::CommandBuffer const&
        operator[](std::size_t index) const
        {
            return command_buffers[index];
        }

        ~command_pool() { get_device().destroyCommandPool(get_command_pool()); }
    };

    struct transfer_pool
        : vk::CommandPool
        , private vk::Device {
    private:
        [[nodiscard]] vk::Device
        get_device() const noexcept
        {
            return static_cast<vk::Device>(*this);
        }

        static vk::CommandPool
        create_command_pool(device const& _device)
        {
            vk::CommandPoolCreateInfo create_info{ vk::CommandPoolCreateFlagBits::eTransient,
                                                   _device.transfer_queue_index };

            return _device.createCommandPool(create_info);
        }

    public:
        explicit transfer_pool(device const& _device)
            : vk::CommandPool(create_command_pool(_device))
            , vk::Device(_device.get_device())
        {
        }

        [[nodiscard]] vk::CommandPool
        get_command_pool() const&
        {
            return static_cast<vk::CommandPool>(*this);
        }

        ~transfer_pool() { get_device().destroyCommandPool(get_command_pool()); }
    };
} // namespace obscure::vulkan