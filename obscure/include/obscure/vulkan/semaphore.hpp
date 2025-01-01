#ifndef OBSCURE_VULKAN_SEMAPHORE_DEFINITION
#define OBSCURE_VULKAN_SEMAPHORE_DEFINITION 1
#include "glfw_vulkan_include.hpp"


namespace obscure
{
    namespace vulkan
    {
        struct semaphore : vk::Semaphore
        {
            private:
                static vk::Semaphore create_semaphore(vk::Device _device)
                {
                    vk::SemaphoreCreateInfo create_info { };

                    return _device.createSemaphore(create_info);
                }

                vk::Device device;
            public:

                semaphore(vk::Device _device)
                    : vk::Semaphore(create_semaphore(_device)), device(_device)
                {}

                [[nodiscard]] vk::Semaphore get() const noexcept
                {
                    return *this;
                }

                [[nodiscard]] vk::Semaphore const* get_ptr() const noexcept
                {
                    return this;
                }

                ~semaphore()
                {
                    device.destroySemaphore(get());
                }
        };
    }
}

#endif