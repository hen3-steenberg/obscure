module;
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
export module obscure.vulkan.transfer_session;
export import obscure.vulkan.device;
export import obscure.vulkan.buffer;
export import obscure.vulkan.texture;

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
        vk::Queue graphics_queue;
        vk::CommandBuffer transfer_command_buffer;
        vk::CommandBuffer graphics_command_buffer;
        uint32_t graphics_queue_family_index;
        uint32_t transfer_queue_family_index;

        transfer_session(obscure::vulkan::device const& _device, vk::CommandPool _command_pool, vk::CommandBuffer gfx_buffer)
            : device(_device.get_device()),
            command_pool(_command_pool),
            transfer_queue(_device.get_transfer_queue()),
            graphics_queue(_device.get_graphics_queue()),
            transfer_command_buffer(create_command_buffer(device, command_pool)),
            graphics_command_buffer(gfx_buffer),
            graphics_queue_family_index(_device.get_graphics_queue_index()),
            transfer_queue_family_index(_device.get_transfer_queue_index())
        {
            vk::CommandBufferBeginInfo begin_info {
                vk::CommandBufferUsageFlagBits::eOneTimeSubmit
            };
            graphics_command_buffer.reset();
            graphics_command_buffer.begin(begin_info);
        }

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

            transfer_command_buffer.copyBuffer(data_src.get_buffer(), data_dest.get_buffer(), copy_region);
        }

        template<
            VkBufferUsageFlags Usage,
            VmaMemoryUsage MemoryUsage,
            VmaAllocationCreateFlags Flags,
            size_t Alignment,
            vk::SamplerAddressMode modeU,
            vk::SamplerAddressMode modeV,
            vk::SamplerAddressMode modeW>
        requires (is_transfer_source<Usage>())
        void copy_buffer_to_texture(buffer_impl<Usage, MemoryUsage, Flags, Alignment> const& data_src, rgba_2d_texture<modeU, modeV, modeW> & data_dst)
        {
            vk::BufferImageCopy copy_region{
                0,
                0,
                0,
                vk::ImageSubresourceLayers{
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    0,
                    1
                },
                vk::Offset3D{0, 0, 0},
                data_dst.extent
            };

            data_dst.template transition_layout<vk::ImageLayout::eTransferDstOptimal>(graphics_command_buffer);
            graphics_command_buffer.copyBufferToImage(data_src.get_buffer(), data_dst.get_image(), vk::ImageLayout::eTransferDstOptimal, 1, &copy_region);
            data_dst.template transition_layout<vk::ImageLayout::eShaderReadOnlyOptimal>(graphics_command_buffer);
        }



        ~transfer_session()
        {


            transfer_command_buffer.end();

            vk::SubmitInfo submit_info {};
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &transfer_command_buffer;

            transfer_queue.submit(submit_info);
            transfer_queue.waitIdle();

            graphics_command_buffer.end();
            vk::SubmitInfo submit_info1 {};
            submit_info1.commandBufferCount = 1;
            submit_info1.pCommandBuffers = &graphics_command_buffer;
            graphics_queue.submit(submit_info1);

            device.freeCommandBuffers(command_pool, 1, &transfer_command_buffer);
        }



    };
}