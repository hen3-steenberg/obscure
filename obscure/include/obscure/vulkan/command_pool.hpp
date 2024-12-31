#ifndef OBSCURE_VULKAN_COMMAND_POOL_DEFINITION
#define OBSCURE_VULKAN_COMMAND_POOL_DEFINITION 1
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/vulkan/device.hpp"



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
                vk::CommandBuffer command_buffer;

                command_pool()
                	: vk::CommandPool(create_command_pool(get_device_ref()))
                {
                    vk::CommandBufferAllocateInfo create_info {
                        get(),
                        vk::CommandBufferLevel::ePrimary,
                        1
                    };

                    auto result = get_device_ref().allocateCommandBuffers(&create_info, &command_buffer);
                }

                [[nodiscard]] vk::CommandPool get() const&
                {
                	return *this;
                }

                ~command_pool()
                {
                	get_device_ref().destroyCommandPool(get());
                }

        };
    }
}
#endif