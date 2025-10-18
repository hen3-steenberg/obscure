module;
#include "vma.hpp"
#include <array>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.msaa_buffer;
export import obscure.vulkan.device;
export import obscure.vulkan.image_view;
import obscure.properties;

export namespace obscure::vulkan
{
    struct msaa_buffer {
        std::array<vk::Image, max_image_count()> sample_images;
        std::array<image_view, max_image_count()> sample_image_views;
        std::array<VmaAllocation, max_image_count()> sample_allocations;
        vk::SampleCountFlagBits msaa_samples;

    private:
        static vk::SampleCountFlagBits
        get_sample_count(vk::PhysicalDevice device) noexcept
        {
            auto properties = device.getProperties();
            vk::SampleCountFlags sample_count =
                properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
            if (sample_count & vk::SampleCountFlagBits::e64)
                return vk::SampleCountFlagBits::e64;
            if (sample_count & vk::SampleCountFlagBits::e32)
                return vk::SampleCountFlagBits::e32;
            if (sample_count & vk::SampleCountFlagBits::e16)
                return vk::SampleCountFlagBits::e16;
            if (sample_count & vk::SampleCountFlagBits::e8)
                return vk::SampleCountFlagBits::e8;
            if (sample_count & vk::SampleCountFlagBits::e4)
                return vk::SampleCountFlagBits::e4;
            if (sample_count & vk::SampleCountFlagBits::e2)
                return vk::SampleCountFlagBits::e2;
            return vk::SampleCountFlagBits::e1;
        }

    public:
        msaa_buffer(device const& device_, vk::Format format, vk::Extent2D extent)
            : msaa_samples(get_sample_count(device_.get_physical_device()))
        {
            for (size_t idx = 0; idx < max_image_count(); ++idx) {
                vk::ImageCreateInfo info{ {},
                                          vk::ImageType::e2D,
                                          format,
                                          vk::Extent3D{ extent, 1 },
                                          1,
                                          1,
                                          msaa_samples,
                                          vk::ImageTiling::eOptimal,
                                          vk::ImageUsageFlagBits::eTransientAttachment |
                                              vk::ImageUsageFlagBits::eColorAttachment,
                                          vk::SharingMode::eExclusive };

                VmaAllocationCreateInfo alloc_info{};
                alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
                alloc_info.flags = 0;

                vmaCreateImage(device_.get_vma_allocator(),
                               reinterpret_cast<VkImageCreateInfo*>(&info),
                               &alloc_info,
                               reinterpret_cast<VkImage*>(&sample_images[idx]),
                               &sample_allocations[idx],
                               nullptr);

                vk::ImageViewCreateInfo view_info{ {},
                                                   sample_images[idx],
                                                   vk::ImageViewType::e2D,
                                                   format,
                                                   {},
                                                   vk::ImageSubresourceRange{
                                                       vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 } };

                sample_image_views[idx] = image_view{ device_.get_device(), sample_images[idx], format };
            }
        }

        void
        free(device const& device_)
        {
            for (size_t idx = 0; idx < max_image_count(); ++idx) {
                sample_image_views[idx].free(device_.get_device());

                vmaDestroyImage(device_.get_vma_allocator(), sample_images[idx], sample_allocations[idx]);
            }
        }
    };
} // namespace obscure::vulkan