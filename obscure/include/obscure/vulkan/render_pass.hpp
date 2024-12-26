#ifndef OBSCURE_VULKAN_RENDER_PASS_DEFINITION
#define OBSCURE_VULKAN_RENDER_PASS_DEFINITION 1
#include "obscure/vulkan/device.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/vulkan/swap_chain.hpp"

namespace obscure {
    namespace vulkan {
        template<typename ... TSiblings>
        struct render_pass : vk::RenderPass
        {
            private:
                device const& get_device_ref() const&
                {
                    return helper_templates::get_parent_ref<const device, TSiblings...>(this);
                }

                static vk::RenderPass create_render_pass(device const& device, swap_chain_ref swap_chain)
                {
                    vk::AttachmentDescription color_attachment {
                        {},
                        swap_chain.get().format,
                        vk::SampleCountFlagBits::e1,
                        vk::AttachmentLoadOp::eClear,
                        vk::AttachmentStoreOp::eStore,
                        vk::AttachmentLoadOp::eDontCare,
                        vk::AttachmentStoreOp::eDontCare,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::ePresentSrcKHR
                    };

                    vk::AttachmentReference color_attachment_ref {
                        0,
                        vk::ImageLayout::eColorAttachmentOptimal
                    };

                    vk::SubpassDescription subpass {
                        {},
                        vk::PipelineBindPoint::eGraphics,
                        0,
                        nullptr,
                        1,
                        &color_attachment_ref,
                        nullptr,
                        nullptr,
                        0,
                        nullptr
                    };

                    vk::RenderPassCreateInfo create_info {
                        {},
                        1,
                        &color_attachment,
                        1,
                        &subpass,
                        0,
                        nullptr
                    };

                    return device.createRenderPass(create_info);
                }
            public:
                  render_pass(swap_chain_ref swap_chain)
                      : vk::RenderPass(create_render_pass(get_device_ref(), swap_chain))
                  {
                  }

                  ~render_pass()
                   {
                        get_device_ref().destroyRenderPass(get());
                   }

                  vk::RenderPass get() const
                  {
                      return *this;
                  }
        };
    }
}
#endif