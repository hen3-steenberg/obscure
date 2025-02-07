#ifndef OBSCURE_VULKAN_DEVICE_DEFINITION
#define OBSCURE_VULKAN_DEVICE_DEFINITION 1
#include "obscure/application_context.hpp"
#include "obscure/obscure_properties.hpp"
#include <functional>
#include <stdexcept>

namespace obscure
{
    namespace vulkan
    {
        inline bool is_device_suitable(vk::PhysicalDevice device, vk::SurfaceKHR surface)
        {
            //check that all required extensions are supported
            auto supported_extensions = device.enumerateDeviceExtensionProperties();

            for (auto required : required_device_extensions())
            {
                bool found = false;
                for (auto extension : supported_extensions)
                {
                    if (strcmp(required, extension.extensionName))
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    return false;
                }
            }

            //check that the device has swap chain support 
            uint32_t format_count = 0;
            device.getSurfaceFormatsKHR(surface, &format_count, nullptr);

            uint32_t mode_count = 0;
            device.getSurfacePresentModesKHR(surface, &mode_count, nullptr);

            //check that all required features are supported.
            auto device_feutures = device.getFeatures();
            return device_feutures.geometryShader && format_count && mode_count;
        }

        inline float get_device_score_default(vk::PhysicalDevice device)
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

        vk::PhysicalDevice pick_device(vk::SurfaceKHR surface, std::function<float (vk::PhysicalDevice)> get_device_score)
        {
            vk::Instance instance = obscure::get_application_instance();
            auto devices = instance.enumeratePhysicalDevices();
            vk::PhysicalDevice result = VK_NULL_HANDLE;
            float score = 0;
            for(auto device : devices)
            {
                if(is_device_suitable(device, surface))
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
            using index_t = std::optional<uint32_t>;
            index_t graphics_queue_index;
            index_t present_queue_index;

            queue_indices(queue_indices const& other) noexcept = default;
            queue_indices(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface)
            {
                const auto queue_families = physical_device.getQueueFamilyProperties();
                for(uint32_t index = 0; index < queue_families.size(); ++index)
                {
                    if(!graphics_queue_index.has_value() && queue_families[index].queueFlags & vk::QueueFlagBits::eGraphics)
                    {
                        graphics_queue_index = index;
                    }

                    if (!present_queue_index.has_value() && physical_device.getSurfaceSupportKHR(index, surface))
                    {
                        present_queue_index = index;
                    }

                    if(is_complete())
                    {
                        break;
                    }
                }
            }

            uint32_t queue_count() const
            {
                if (graphics_queue_index.has_value() && present_queue_index.has_value())
                {
                    return (graphics_queue_index.value() == present_queue_index.value()) ? 1 : 2;
                }
                else if (graphics_queue_index.has_value() || present_queue_index.has_value())
                {
                    return 1;
                }
                else return 0;
            }

            bool is_complete() const noexcept
            {
                return graphics_queue_index.has_value() && present_queue_index.has_value();
            }
        };

        

        struct device : vk::Device {
            vk::PhysicalDevice physical_device;
            uint32_t graphics_queue_index;
            uint32_t present_queue_index;

            private :

            static vk::Device create_logical_device(vk::PhysicalDevice physical_device, queue_indices indices)
            {
                static constexpr float priority = 1.0f;

                vk::PhysicalDeviceFeatures required_features{};

                std::array<vk::DeviceQueueCreateInfo, 2> queue_infos =
                {
                    vk::DeviceQueueCreateInfo
                    {
                            {},
                            indices.graphics_queue_index.value(),
                            1,
                            &priority,
                        },
                        vk::DeviceQueueCreateInfo
                        {
                            {},
                            indices.present_queue_index.value(),
                            1,
                            &priority,
                        }
                };

                constexpr auto extensions = required_device_extensions();
                constexpr uint32_t extension_count = static_cast<uint32_t>(extensions.size());


                if constexpr (enable_debug_validation())
                {
                    const char* debug_layer_name = "VK_LAYER_KHRONOS_validation";
                    vk::DeviceCreateInfo create_info
                    {
                            {},
                            indices.queue_count(),
                            queue_infos.data(),
                            1,
                            &debug_layer_name,
                            extension_count,
                            extensions.data(),
                            &required_features

                        };

                    return physical_device.createDevice(create_info);
                }
                else
                {
                    vk::DeviceCreateInfo create_info
                    {
                            {},
                            indices.queue_count(),
                            queue_infos.data(),
                            0,
                            nullptr,
                            extension_count,
                            extensions.data(),
                            &required_features

                        };
                    return physical_device.createDevice(create_info);
                }
            }

            device(vk::PhysicalDevice physical_device, queue_indices indices)
                : vk::Device(create_logical_device(physical_device, indices)), physical_device(physical_device),
                graphics_queue_index(indices.graphics_queue_index.value()),
                present_queue_index(indices.present_queue_index.value())
            {
            }

        public:

            device (vk::PhysicalDevice physical_device, vk::SurfaceKHR surface)
                :device(physical_device, queue_indices{physical_device, surface})
            {}

            device(vk::SurfaceKHR surface, std::function<float(vk::PhysicalDevice)> get_device_score)
                : device(pick_device(surface, std::move(get_device_score)), surface)
            {}

            device(device && other) noexcept
                : vk::Device(other.get_device()),
                physical_device(other.physical_device),
                graphics_queue_index(other.graphics_queue_index),
                present_queue_index(other.present_queue_index) {
                static_cast<vk::Device&>(other) = VK_NULL_HANDLE;
            }


            [[nodiscard]] vk::Device get_device() const noexcept
            {
                return static_cast<vk::Device>(*this);
            }

            [[nodiscard]] vk::PhysicalDevice get_physical_device() const noexcept
            {
                return physical_device;
            }

            [[nodiscard]] uint32_t get_graphics_queue_index() const noexcept
            {
                return graphics_queue_index;
            }

            [[nodiscard]] uint32_t get_present_queue_index() const noexcept
            {
                return present_queue_index;
            }

            [[nodiscard]] vk::Queue get_graphics_queue() const
            {
                return get_device().getQueue(graphics_queue_index, 0);
            }

            [[nodiscard]] vk::Queue get_present_queue() const
            {
                return get_device().getQueue(present_queue_index, 0);
            }

            ~device()
            {
                if (get_device() != VK_NULL_HANDLE) {
                    destroy();
                }
            }
        };


    }
}
#endif