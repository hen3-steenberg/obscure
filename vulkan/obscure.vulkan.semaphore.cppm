module;
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.semaphore;

export namespace obscure::vulkan
{
    struct semaphore : vk::Semaphore {
      private:
        static vk::Semaphore
        create_semaphore(vk::Device _device)
        {
            vk::SemaphoreCreateInfo create_info{};

            return _device.createSemaphore(create_info);
        }

      public:
        semaphore(vk::Device _device)
            : vk::Semaphore(create_semaphore(_device))
        {
        }

        [[nodiscard]] vk::Semaphore
        get_semaphore() const noexcept
        {
            return *this;
        }

        [[nodiscard]] vk::Semaphore const*
        get_semaphore_ptr() const noexcept
        {
            return this;
        }

        void
        free(vk::Device device)
        {
            device.destroySemaphore(get_semaphore());
        }
    };
} // namespace obscure::vulkan