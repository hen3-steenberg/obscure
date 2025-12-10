module;
#include "vma.hpp"
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.buffer:uniform;
import :impl;

import obscure.properties;
import obscure.helper_templates.array_initialize;
import obscure.vulkan.result;

export namespace obscure::vulkan
{
    template<typename T>
    using uniform_buffer_impl =
        buffer_impl<VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                    VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    std::alignment_of_v<T>>;

    template<typename T>
    struct uniform_buffer {
        using buffer_set = std::array<uniform_buffer_impl<T>, obscure::max_image_count()>;
        buffer_set data;
        vk::DescriptorPool descriptor_pool;
        std::array<vk::DescriptorSet, obscure::max_image_count()> descriptor_sets;
        T* current_buffer;

    private:
        static buffer_set
        make_buffer_set(const device& _device)
        {
            return obscure::helper_templates::
                initialize_array<uniform_buffer_impl<T>, obscure::max_image_count(), decltype(_device), size_t>(
                    _device, sizeof(T));
        }

        static vk::DescriptorPool
        create_descriptor_pool(const device& device)
        {
            vk::DescriptorPoolSize pool_size{ vk::DescriptorType::eUniformBuffer, obscure::max_image_count() };

            vk::DescriptorPoolCreateInfo create_info{ {}, obscure::max_image_count(), 1, &pool_size };

            return device.createDescriptorPool(create_info);
        }

        std::array<vk::DescriptorSet, obscure::max_image_count()>
        create_descriptor_sets(const device& device, vk::DescriptorPool pool, vk::DescriptorSetLayout layout)
        {
            std::array<vk::DescriptorSetLayout, obscure::max_image_count()> layouts{};
            layouts.fill(layout);
            std::array<vk::DescriptorSet, obscure::max_image_count()> result{};

            vk::DescriptorSetAllocateInfo alloc_info{ pool, layouts };

            check(device.allocateDescriptorSets(&alloc_info, result.data()));

            return result;
        }

    public:
        explicit uniform_buffer(const device& device, vk::DescriptorSetLayout layout, uint32_t binding)
            : data(make_buffer_set(device))
            , descriptor_pool(create_descriptor_pool(device))
            , descriptor_sets(create_descriptor_sets(device, descriptor_pool, layout))
            , current_buffer(nullptr)
        {

            for (size_t idx = 0; idx < obscure::max_image_count(); ++idx) {
                vk::DescriptorBufferInfo buffer_info{ data[idx].get_buffer(), 0, sizeof(T) };

                vk::WriteDescriptorSet write_descriptor_set{
                    descriptor_sets[idx], binding, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr,
                    &buffer_info,         nullptr
                };

                device.updateDescriptorSets(write_descriptor_set, {});
            }
        }

        void
        set_current_index(size_t idx)
        {
            T* prev = current_buffer;
            current_buffer = data[idx].template get_mapped_ptr<T>();
            if (prev) {
                memcpy(current_buffer, prev, sizeof(T));
            } else {
                new (current_buffer) T{};
            }
        }

        [[nodiscard]] T&
        get() noexcept
        {
            return *current_buffer;
        }

        [[nodiscard]] T const&
        get() const noexcept
        {
            return *current_buffer;
        }

        [[nodiscard]] T&
        operator*() noexcept
        {
            return *current_buffer;
        }

        [[nodiscard]] T const&
        operator*() const noexcept
        {
            return *current_buffer;
        }

        [[nodiscard]] T*
        operator->() noexcept
        {
            return current_buffer;
        }

        [[nodiscard]] T const*
        operator->() const noexcept
        {
            return current_buffer;
        }

        [[nodiscard]] T*
        operator&() noexcept
        {
            return current_buffer;
        }

        [[nodiscard]] T const*
        operator&() const noexcept
        {
            return current_buffer;
        }

        ~uniform_buffer()
        {
            data[0].vk_device->waitIdle();
            current_buffer->~T();
            data[0].vk_device->destroyDescriptorPool(descriptor_pool);
        }
    };
} // namespace obscure::vulkan