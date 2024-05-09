#ifndef OBSCURE_VULKAN_DEVICE_DEFINITION
#define OBSCURE_VULKAN_DEVICE_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/obscure_properties.hpp"
#include <functional>
#include <stdexcept>

namespace obscure
{
    namespace vulkan
    {
        bool is_device_suitable(vk::PhysicalDevice device)
        {
            auto device_feutures = device.getFeatures();
            return device_feutures.geometryShader;
        }

        float get_device_score_default(vk::PhysicalDevice device)
        {
            float device_score = 0;
            auto device_properties = device.getProperties();
            switch (device_properties.deviceType) {
                case vk::PhysicalDeviceType::eDiscreteGpu:
                    device_score += 100.0f;
                    break;
                case vk::PhysicalDeviceType::eIntegratedGpu:
                    device_score += 10.0f;
                    break;                     
                case vk::PhysicalDeviceType::eVirtualGpu:
                    device_score += 1.0f;
                    break;
                case vk::PhysicalDeviceType::eCpu:
                    device_score += 0.1f;
                    break;
                case vk::PhysicalDeviceType::eOther:
                default:
                    break;
            }
            return device_score;
        }

        vk::PhysicalDevice pick_device(vk::Instance const& instance, std::function<float (vk::PhysicalDevice)> get_device_score)
        {
            auto devices = instance.enumeratePhysicalDevices();
            vk::PhysicalDevice result = VK_NULL_HANDLE;
            float score = 0;
            for(auto device : devices)
            {
                if(is_device_suitable(device))
                {
                    float device_score = get_device_score(device) + 1.0f;

                    if(device_score > score)
                    {
                        result = device;
                        score = device_score;
                    }
                }
            }
            if(score)
            {
                return result;
            }
            else 
            {
                throw std::runtime_error("No suitable devices found.");
            }
            
        }

        struct queue_indices
        {
            std::optional<uint32_t> graphics_queue_index;

            queue_indices(queue_indices const& other) noexcept = default;
            queue_indices(vk::PhysicalDevice physical_device)
            {
                const auto queue_families = physical_device.getQueueFamilyProperties();
                for(uint32_t index = 0; index < queue_families.size(); ++index)
                {
                    if(!graphics_queue_index.has_value() && queue_families[index].queueFlags & vk::QueueFlagBits::eGraphics)
                    {
                        graphics_queue_index = index;
                    }

                    if(is_complete())
                    {
                        break;
                    }
                }
            }

            bool is_complete() const noexcept
            {
                return graphics_queue_index.has_value();
            }
        };

        

        struct device : vk::Device
        {
            vk::PhysicalDevice physical_device;
            uint32_t graphics_queue_index;

            private :

            static vk::Device create_logical_device(vk::PhysicalDevice physical_device, queue_indices indices)
            {
                static constexpr float priority = 1.0f;

                vk::PhysicalDeviceFeatures required_features{};

                vk::DeviceQueueCreateInfo graphic_queue_info
                {
                    {},
                    indices.graphics_queue_index.value(),
                    1,
                    &priority,
                };

                if constexpr (enable_debug_validation())
                {
                    const char* debug_layer_name = "VK_LAYER_KHRONOS_validation";
                    vk::DeviceCreateInfo create_info
                    {
                        {},
                        1,
                        &graphic_queue_info,
                        1,
                        &debug_layer_name,
                        0,
                        nullptr,
                        &required_features

                    };

                    return physical_device.createDevice(create_info);
                }
                else 
                {
                    vk::DeviceCreateInfo create_info
                    {
                        {},
                        1,
                        &graphic_queue_info,
                        0,
                        nullptr,
                        0,
                        nullptr,
                        &required_features

                    };
                    return physical_device.createDevice(create_info);
                }
            }

            device(vk::PhysicalDevice physical_device, queue_indices indices)
                : vk::Device(create_logical_device(physical_device, indices)), physical_device(physical_device),
                graphics_queue_index(indices.graphics_queue_index.value())
            {
            }

            public:

            device(vk::PhysicalDevice physical_device)
                : device(physical_device, queue_indices{physical_device})
            {}

            

            ~device()
            {
                destroy();
            }
        };


    }
}
#endif