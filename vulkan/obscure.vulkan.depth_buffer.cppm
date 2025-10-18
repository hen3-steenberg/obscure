module;
#include "vma.hpp"
#include <array>
#include <tuple>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.depth_buffer;
export import obscure.vulkan.device;
import obscure.properties;

export namespace obscure::vulkan
{
    struct depth_buffer {
        constexpr static size_t buffer_count = obscure::max_image_count();
        vk::Format format;
        std::array<vk::Image, buffer_count> images;
        std::array<vk::ImageView, buffer_count> views;
        std::array<VmaAllocation, buffer_count> allocations;

    private:
        static vk::Format
        get_format(device const& device_)
        {
            static constexpr std::array formats = { vk::Format::eD32SfloatS8Uint,
                                                    vk::Format::eD24UnormS8Uint,
                                                    vk::Format::eD32Sfloat,
                                                    vk::Format::eD16UnormS8Uint,
                                                    vk::Format::eD16Unorm };
            for (vk::Format format : formats) {
                auto props = device_.get_physical_device().getFormatProperties(format);
                if (props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
                    return format;
                }
            }
            for (vk::Format format : formats) {
                auto props = device_.get_physical_device().getFormatProperties(format);
                if (props.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
                    return format;
                }
            }
            return vk::Format::eD32Sfloat;
        }

        static std::tuple<vk::Image, vk::ImageView, VmaAllocation>
        allocate_image(device const& device_, vk::Format format, vk::Extent3D extent, vk::SampleCountFlagBits samples)
        {
            std::tuple<vk::Image, vk::ImageView, VmaAllocation> result{};
            vk::ImageCreateInfo info{ {},
                                      vk::ImageType::e2D,
                                      format,
                                      extent,
                                      1,
                                      1,
                                      samples,
                                      vk::ImageTiling::eOptimal,
                                      vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                      vk::SharingMode::eExclusive };

            VmaAllocationCreateInfo alloc_info{};
            alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

            vmaCreateImage(device_.get_vma_allocator(),
                           reinterpret_cast<VkImageCreateInfo const*>(&info),
                           &alloc_info,
                           reinterpret_cast<VkImage*>(&std::get<0>(result)),
                           &std::get<2>(result),
                           nullptr);

            vk::ImageViewCreateInfo view_info{ {},
                                               std::get<0>(result),
                                               vk::ImageViewType::e2D,
                                               format,
                                               {},
                                               vk::ImageSubresourceRange{
                                                   vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 } };

            std::get<1>(result) = device_.createImageView(view_info);
            return result;
        }

    public:
        depth_buffer(device const& device_, vk::Extent3D extent, vk::SampleCountFlagBits samples)
            : format(get_format(device_))
        {
            for (size_t idx = 0; idx < buffer_count; ++idx) {
                auto [img, view, alloc] = allocate_image(device_, format, extent, samples);
                images[idx] = img;
                views[idx] = view;
                allocations[idx] = alloc;
            }
        }

        void
        free(device const& device_) noexcept
        {
            device_.waitIdle();
            for (size_t idx = 0; idx < buffer_count; ++idx) {
                device_.destroyImageView(views[idx]);
                vmaDestroyImage(device_.get_vma_allocator(), images[idx], allocations[idx]);
            }
        }
    };
} // namespace obscure::vulkan