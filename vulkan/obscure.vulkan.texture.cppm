module;
#include <tuple>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
export module obscure.vulkan.texture;
export import obscure.vulkan.buffer;
export import obscure.vulkan.device;

export namespace obscure::vulkan {

    template<vk::ImageType type, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageTiling tiling, VmaMemoryUsage MemoryUsage, VmaAllocationCreateFlags Flags>

    struct texture_impl : vk::Image {

    private:
        static std::pair<vk::Image, VmaAllocation> create_texture(device const& _device, vk::Extent3D extent) {
            vk::ImageCreateInfo info{
                {},
                type,
                format,
                extent,
                1,
                1,
                samples,
                tiling,
                vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                vk::SharingMode::eExclusive
            };

            VmaAllocationCreateInfo alloc_info{};
            alloc_info.usage = MemoryUsage;
            alloc_info.flags = Flags;

            std::pair<vk::Image, VmaAllocation> result{VK_NULL_HANDLE, nullptr};

            vmaCreateImage(
                _device.get_vma_allocator(),
                reinterpret_cast<VkImageCreateInfo*>(&info),
                &alloc_info,
                reinterpret_cast<VkImage*>(&result.first),
                &result.second,
                nullptr);

            return result;
        }



        texture_impl(const device& _device, std::pair<vk::Image, VmaAllocation> alloced_image, vk::Extent3D _extent)
            : vk::Image{alloced_image.first},
            allocation(alloced_image.second),
            vk_device(&_device),
            extent(_extent)
        {}

    public:

        texture_impl() noexcept
            : vk::Image{VK_NULL_HANDLE},
            allocation(nullptr),
            vk_device(nullptr),
            extent()
        {}

        texture_impl(const device& _device, vk::Extent3D _extent)
            : texture_impl(_device, create_texture(_device, _extent), _extent)
        {}

        texture_impl(const texture_impl& ) = delete;
        texture_impl(texture_impl&& other) noexcept
            : vk::Image{other.get_image()},
            allocation(other.allocation),
            vk_device(other.vk_device),
            extent(other.extent)
        {
            other.vk_device = nullptr;
        }

        VmaAllocation allocation{};
        const device * vk_device;
        vk::Extent3D extent;

        vk::Image get_image() const noexcept {
            return static_cast<vk::Image>(*this);
        }

        ~texture_impl() {
            if (vk_device) {
                vmaDestroyImage(
                    vk_device->get_vma_allocator(),
                    get_image(),
                    allocation);
            }
        }
    };

    using rgba_2d_texture_impl = texture_impl<
        vk::ImageType::e2D,
    vk::Format::eR8G8B8A8Srgb,
    vk::SampleCountFlagBits::e1,
    vk::ImageTiling::eOptimal,
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    0>;

    struct rgba_2d_texture : rgba_2d_texture_impl
    {
    private:
        template<vk::ImageLayout new_layout>
        static consteval bool can_transition()
        {
            return new_layout == vk::ImageLayout::eTransferDstOptimal ||
                new_layout == vk::ImageLayout::eShaderReadOnlyOptimal;
        }
    public:
        vk::ImageLayout image_layout = vk::ImageLayout::eUndefined;

        rgba_2d_texture(const device& _device, vk::Extent3D _extent)
            : rgba_2d_texture_impl(_device, _extent)
        {}

        template<vk::ImageLayout new_layout>
            requires (can_transition<new_layout>())
        void transition_layout(vk::CommandBuffer cmd, uint32_t srcQueue = VK_QUEUE_FAMILY_IGNORED, uint32_t dstQueue = VK_QUEUE_FAMILY_IGNORED) {


            vk::ImageMemoryBarrier barrier{
                {},
                {},
                image_layout,
                new_layout,
                srcQueue,
                dstQueue,
                get_image(),
                vk::ImageSubresourceRange {
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1
                }
            };

            vk::PipelineStageFlags sourceStage{};
            vk::PipelineStageFlags destinationStage{};

            if (new_layout == vk::ImageLayout::eTransferDstOptimal && image_layout == vk::ImageLayout::eUndefined)
            {
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eTransfer;
            }
            else if (new_layout == vk::ImageLayout::eShaderReadOnlyOptimal && image_layout == vk::ImageLayout::eTransferDstOptimal)
            {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

                sourceStage = vk::PipelineStageFlagBits::eTransfer;
                destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else
            {
                return;
            }

            cmd.pipelineBarrier(
                sourceStage,
                destinationStage,
                {},
                0,
                nullptr,
                0,
                nullptr,
                1,
                &barrier);
            image_layout = new_layout;
        }
    };
}