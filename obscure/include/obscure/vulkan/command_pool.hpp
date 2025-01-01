#ifndef OBSCURE_VULKAN_COMMAND_POOL_DEFINITION
#define OBSCURE_VULKAN_COMMAND_POOL_DEFINITION 1
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/vulkan/device.hpp"
#include <vector>



namespace obscure
{
    namespace vulkan
    {
        template<typename ... TSiblings>
        struct command_pool : vk::CommandPool
        {
            private:
                device const& get_device_ref() const
                {
                    return obscure::helper_templates::get_parent_ref<const device, TSiblings...>(this);
                }

                static vk::CommandPool create_command_pool(device const & _device)
                {
					vk::CommandPoolCreateInfo create_info {
                    	vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                        _device.graphics_queue_index
                    };

                    return _device.createCommandPool(create_info);
                }
            public:
                std::vector<vk::CommandBuffer> command_buffers;

                explicit command_pool(std::size_t buffer_count)
                	: vk::CommandPool(create_command_pool(get_device_ref()))
                {
                    vk::CommandBufferAllocateInfo create_info {
                        get(),
                        vk::CommandBufferLevel::ePrimary,
                        static_cast<uint32_t>(buffer_count)
                    };

                    command_buffers = get_device_ref().allocateCommandBuffers(create_info);
                }

                [[nodiscard]] vk::CommandPool get() const&
                {
                	return *this;
                }

                [[nodiscard]] vk::CommandBuffer const& operator[](std::size_t index) const
                {
                    return command_buffers[index];
                }

                ~command_pool()
                {
                	get_device_ref().destroyCommandPool(get());
                }

        };
    }
}
#endif