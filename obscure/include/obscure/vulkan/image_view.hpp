#ifndef OBSCURE_VULKAN_IMAGE_VIEW_DEFINITION
#define OBSCURE_VULKAN_IMAGE_VIEW_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include <vector>


namespace obscure
{
	namespace vulkan
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
}

#endif