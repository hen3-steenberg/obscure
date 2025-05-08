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
            public:

                semaphore(vk::Device _device)
                    : vk::Semaphore(create_semaphore(_device))
                {}

                [[nodiscard]] vk::Semaphore get_semaphore() const noexcept
                {
                    return *this;
                }

                [[nodiscard]] vk::Semaphore const* get_semaphore_ptr() const noexcept
                {
                    return this;
                }

                void free(vk::Device device)
                {
                    device.destroySemaphore(get_semaphore());
                }
        };
    }
}

#endif