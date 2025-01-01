#ifndef OBSCURE_VULKAN_FENCE_DEFINITION
#define OBSCURE_VULKAN_FENCE_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include <limits>

namespace obscure
{
    namespace vulkan
    {
        struct fence : vk::Fence
        {
            private:
                static vk::Fence create_fence(vk::Device _device, bool signaled)
                {
                    static constexpr const vk::FenceCreateFlags SignaledFlag { vk::FenceCreateFlagBits::eSignaled  };
                    static constexpr const vk::FenceCreateFlags NotSignaledFlag { };
                    vk::FenceCreateFlags flags = signaled ? SignaledFlag : NotSignaledFlag;
                    vk::FenceCreateInfo create_info { flags };

                    return _device.createFence(create_info);
                }

                vk::Device device;
            public:

                fence(vk::Device _device, bool signaled = true)
                    : vk::Fence(create_fence(_device, signaled)), device(_device)
                {}

                [[nodiscard]] vk::Fence get() const noexcept
                {
                    return static_cast<vk::Fence>(*this);
                }

                void wait(uint64_t timeout = std::numeric_limits<uint64_t>::max())
                {
                    auto _ = device.waitForFences(1, this, vk::True, timeout);
                }

                void reset()
                {
                    auto _ = device.resetFences(1, this);
                }

                void wait_and_reset(uint64_t timeout = std::numeric_limits<uint64_t>::max())
                {
                    wait(timeout);
                    reset();
                }


                ~fence()
                {
                    if (get() != VK_NULL_HANDLE)
                    {
                        device.destroyFence(get());
                    }
                }
        };
    }
}

#endif