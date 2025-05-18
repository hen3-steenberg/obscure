module;
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
export module obscure.vulkan.transfer_session;
export import obscure.vulkan.buffer;

export namespace obscure::vulkan
{
    struct transfer_session
    {
    private:
        static vk::CommandBuffer create_command_buffer(vk::Device device, vk::CommandPool pool)
        {
            vk::CommandBufferAllocateInfo create_info {
                pool,
                vk::CommandBufferLevel::ePrimary,
                1
            };

            vk::CommandBuffer copyCmd{};
            device.allocateCommandBuffers(&create_info, &copyCmd);

            vk::CommandBufferBeginInfo begin_info {
                vk::CommandBufferUsageFlagBits::eOneTimeSubmit
            };

            copyCmd.begin(begin_info);

            return copyCmd;
        }
    public:
        vk::Device device;
        vk::CommandPool command_pool;
        vk::Queue transfer_queue;
        vk::CommandBuffer command_buffer;

        transfer_session(vk::Device _device, vk::CommandPool _command_pool, vk::Queue _transfer_queue)
            : device(_device),
            command_pool(_command_pool),
            transfer_queue(_transfer_queue),
            command_buffer(create_command_buffer(device, command_pool))
        {}

        template<
            VkBufferUsageFlags Usage1,
            VmaMemoryUsage MemoryUsage1,
            VmaAllocationCreateFlags Flags1,
            VkBufferUsageFlags Usage2,
            VmaMemoryUsage MemoryUsage2,
            VmaAllocationCreateFlags Flags2,
            size_t Alignment>
        requires (is_transfer_source<Usage1>() && is_transfer_destination<Usage2>())
        void transfer_data(
            buffer_impl<Usage1, MemoryUsage1, Flags1, Alignment> const& data_src,
            buffer_impl<Usage2, MemoryUsage2, Flags2, Alignment> const& data_dest,
            vk::DeviceSize size = VK_WHOLE_SIZE)
        {
            if (size == VK_WHOLE_SIZE)
            {
                size = std::min(data_src.size(), data_dest.size());
            }

            if constexpr (can_write<Flags1>())
            {
                vmaFlushAllocation(
                    data_src.vk_device->get_vma_allocator(),
                    data_src.allocation,
                0, VK_WHOLE_SIZE);
            }

            vk::BufferCopy copy_region{
                0,
                0,
                size
            };

            command_buffer.copyBuffer(data_src.get_buffer(), data_dest.get_buffer(), copy_region);
        }


        ~transfer_session()
        {
            command_buffer.end();

            vk::SubmitInfo submit_info {};
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &command_buffer;

            transfer_queue.submit(1, &submit_info, VK_NULL_HANDLE);
            transfer_queue.waitIdle();

            device.freeCommandBuffers(command_pool, 1, &command_buffer);
        }



    };
}