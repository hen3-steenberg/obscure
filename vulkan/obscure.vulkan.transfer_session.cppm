module;
#include <vulkan/vulkan.hpp>
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