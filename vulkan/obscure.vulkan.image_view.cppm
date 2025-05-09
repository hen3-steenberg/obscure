module;
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
export module obscure.vulkan.image_view;

export namespace obscure::vulkan
{
    struct image_view : vk::ImageView
    {
    private:
        static vk::ImageView create_image_view(vk::Device device, vk::Image image, vk::Format format)
        {
            if (image) {
                vk::ImageViewCreateInfo create_info
                {
						        {},
                                image,
                                vk::ImageViewType::e2D,
                                format,
                                vk::ComponentMapping
                                {
                                    vk::ComponentSwizzle::eIdentity,
                                    vk::ComponentSwizzle::eIdentity,
                                    vk::ComponentSwizzle::eIdentity,
                                    vk::ComponentSwizzle::eIdentity
                                },
                                vk::ImageSubresourceRange
                                {
                                    vk::ImageAspectFlagBits::eColor,
                                    0,
                                    1,
                                    0,
                                    1
                                }
                };
                return device.createImageView(create_info);
            }
            else return VK_NULL_HANDLE;

        }

    public:
        image_view()
            : vk::ImageView(VK_NULL_HANDLE)
        {}

        image_view(vk::Device device, vk::Image image, vk::Format format)
            : vk::ImageView(create_image_view(device, image, format))
        {}

        [[nodiscard]] vk::ImageView get() const&
        {
            return static_cast<vk::ImageView>(*this);
        }

        void free(vk::Device device) noexcept
        {
            if (get() != VK_NULL_HANDLE) {
                device.destroyImageView(get());
            }
        }
    };
}