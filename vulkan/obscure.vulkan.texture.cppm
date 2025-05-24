module;
#include <tuple>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <vulkan/vulkan.hpp>
#include "vk_mem_alloc.h"
export module obscure.vulkan.texture;
export import obscure.vulkan.buffer;
export import obscure.vulkan.device;
import obscure.properties;

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

    template<
        vk::SamplerAddressMode modeU = vk::SamplerAddressMode::eRepeat,
        vk::SamplerAddressMode modeV = modeU,
        vk::SamplerAddressMode modeW = modeV>
    struct rgba_2d_texture : rgba_2d_texture_impl
    {
    private:
        template<vk::ImageLayout new_layout>
        static consteval bool can_transition()
        {
            return new_layout == vk::ImageLayout::eTransferDstOptimal ||
                new_layout == vk::ImageLayout::eShaderReadOnlyOptimal;
        }

        static vk::ImageView create_image_view(vk::Device device_, vk::Image image)
        {
            vk::ImageViewCreateInfo view_info{
                {},
                image,
                vk::ImageViewType::e2D,
                vk::Format::eR8G8B8A8Srgb,
                {},
                vk::ImageSubresourceRange {
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1
                }
            };

            return device_.createImageView(view_info);
        }

        static vk::Sampler create_sampler(device const& device_)
        {

            auto props = device_.get_physical_device().getProperties();

            vk::SamplerCreateInfo sampler_info{
                {},
                vk::Filter::eLinear,
                vk::Filter::eLinear,
                vk::SamplerMipmapMode::eLinear,
                modeU,
                modeV,
                modeW,
                0.0f,
                vk::False,
                props.limits.maxSamplerAnisotropy,
                vk::False,
                vk::CompareOp::eAlways,
                0.0f,
                0.0f,
                vk::BorderColor::eFloatTransparentBlack,
                vk::False
            };

            return device_.createSampler(sampler_info);
        }

        static vk::DescriptorPool create_descriptor_pool(const device& device)
        {
            vk::DescriptorPoolSize pool_size{
                vk::DescriptorType::eCombinedImageSampler,
                obscure::max_image_count()
            };

            vk::DescriptorPoolCreateInfo create_info{
                        {},
                        obscure::max_image_count(),
                        1,
                        &pool_size
                    };

            return device.createDescriptorPool(create_info);
        }

        std::array<vk::DescriptorSet, obscure::max_image_count()> create_descriptor_sets(const device& device, vk::DescriptorPool pool, vk::DescriptorSetLayout layout)
        {
            std::array<vk::DescriptorSetLayout, obscure::max_image_count()> layouts{};
            layouts.fill(layout);
            std::array<vk::DescriptorSet, obscure::max_image_count()> result{};

            vk::DescriptorSetAllocateInfo alloc_info{
                pool,
                layouts
            };

            device.allocateDescriptorSets(&alloc_info, result.data());

            return result;

        }
    public:
        vk::ImageView texture_view;
        vk::Sampler texture_sampler;
        vk::DescriptorPool descriptor_pool;
        std::array<vk::DescriptorSet, obscure::max_image_count()> descriptor_sets;
        vk::ImageLayout image_layout = vk::ImageLayout::eUndefined;


        rgba_2d_texture(const device& _device, vk::Extent3D _extent, vk::DescriptorSetLayout layout, uint32_t binding)
            : rgba_2d_texture_impl(_device, _extent),
            texture_view(create_image_view(_device.get_device(), get_image())),
            texture_sampler(create_sampler(_device)),
            descriptor_pool(create_descriptor_pool(_device)),
            descriptor_sets(create_descriptor_sets(_device, descriptor_pool, layout))
        {

            vk::DescriptorImageInfo image_info{
                texture_sampler,
                texture_view,
                vk::ImageLayout::eShaderReadOnlyOptimal
            };

            for (size_t idx = 0; idx < obscure::max_image_count(); ++idx)
            {

                vk::WriteDescriptorSet write_descriptor_set{
                    descriptor_sets[idx],
                    binding,
                    0,
                    1,
                    vk::DescriptorType::eCombinedImageSampler,
                    &image_info,
                    nullptr,
                    nullptr
                };

                _device.updateDescriptorSets(write_descriptor_set, {});
            }
        }

        rgba_2d_texture(rgba_2d_texture&& other) noexcept
            : rgba_2d_texture_impl(std::move(other)),
            texture_view(other.texture_view),
            texture_sampler(other.texture_sampler),
            descriptor_pool(other.descriptor_pool),
            descriptor_sets(other.descriptor_sets),
            image_layout(other.image_layout)
        {}

        template<vk::ImageLayout new_layout>
            requires (can_transition<new_layout>())
        void transition_layout(vk::CommandBuffer cmd) {


            vk::ImageMemoryBarrier barrier{
                {},
                {},
                image_layout,
                new_layout,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
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

        ~rgba_2d_texture()
        {
            if (vk_device)
            {
                vk_device->waitIdle();
                vk_device->destroyDescriptorPool(descriptor_pool);
                vk_device->destroySampler(texture_sampler);
                vk_device->destroyImageView(texture_view);
            }
        }
    };
}