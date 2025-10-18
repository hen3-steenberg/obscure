module;
#include <chrono>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.fence;

export namespace obscure::vulkan
{
    struct fence : vk::Fence {
      private:
        static vk::Fence
        create_fence(vk::Device _device, bool signaled)
        {
            static constexpr const vk::FenceCreateFlags SignaledFlag{ vk::FenceCreateFlagBits::eSignaled };
            static constexpr const vk::FenceCreateFlags NotSignaledFlag{};
            vk::FenceCreateFlags flags = signaled ? SignaledFlag : NotSignaledFlag;
            vk::FenceCreateInfo create_info{ flags };

            return _device.createFence(create_info);
        }

      public:
        fence(vk::Device _device, bool signaled = true)
            : vk::Fence(create_fence(_device, signaled))
        {
        }

        [[nodiscard]] vk::Fence
        get_fence() const noexcept
        {
            return static_cast<vk::Fence>(*this);
        }

        void
        wait(vk::Device device)
        {
            auto _ = device.waitForFences(1, this, vk::True, std::numeric_limits<std::uint64_t>::max());
        }

        template<typename Rep, typename Period>
        void
        wait(vk::Device device, std::chrono::duration<Rep, Period> timeout)
        {
            using duration_t = std::chrono::duration<uint64_t, std::nano>;
            duration_t nanos = std::chrono::duration_cast<duration_t>(timeout);
            auto _ = device.waitForFences(1, this, vk::True, nanos.count());
        }

        void
        reset(vk::Device device)
        {
            auto _ = device.resetFences(1, this);
        }

        void
        wait_and_reset(vk::Device device)
        {
            wait(device);
            reset(device);
        }

        template<typename Rep, typename Period>
        void
        wait_and_reset(vk::Device device, std::chrono::duration<Rep, Period> timeout)
        {
            wait(device, timeout);
            reset(device);
        }

        void
        free(vk::Device device)
        {
            if (get_fence() != VK_NULL_HANDLE) {
                device.destroyFence(get_fence());
                static_cast<vk::Fence&>(*this) = VK_NULL_HANDLE;
            }
        }
    };
} // namespace obscure::vulkan