module;
#include "vma.hpp"
#include <functional>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.device;
import obscure.vulkan.application_context;
import obscure.properties;
import obscure.vulkan.result;

namespace obscure::vulkan
{
    inline bool
    is_device_suitable(vk::PhysicalDevice device, vk::SurfaceKHR surface)
    {
        // check that all required extensions are supported
        auto supported_extensions = device.enumerateDeviceExtensionProperties();

        for (auto required : required_device_extensions()) {
            bool found = false;
            for (auto extension : supported_extensions) {
                if (strcmp(required, extension.extensionName)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }

        // check that the device has swap chain support
        uint32_t format_count = 0;
        check(device.getSurfaceFormatsKHR(surface, &format_count, nullptr));

        uint32_t mode_count = 0;
        check(device.getSurfacePresentModesKHR(surface, &mode_count, nullptr));

        // check that all required features are supported.
        auto device_feutures = device.getFeatures();
        return device_feutures.geometryShader && format_count && mode_count;
    }

    export inline float
    get_device_score_default(vk::PhysicalDevice device)
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

    vk::PhysicalDevice
    pick_device(vk::SurfaceKHR surface, std::function<float(vk::PhysicalDevice)> get_device_score)
    {
        vk::Instance instance = obscure::get_application_instance();
        auto devices = instance.enumeratePhysicalDevices();
        vk::PhysicalDevice result = VK_NULL_HANDLE;
        float score = 0;
        for (auto device : devices) {
            if (is_device_suitable(device, surface)) {
                float device_score = get_device_score(device) + 1.0f;

                if (device_score > score) {
                    result = device;
                    score = device_score;
                }
            }
        }
        if (score) {
            return result;
        } else {
            throw std::runtime_error("No suitable devices found.");
        }
    }

    export struct queue_indices {
        using index_t = std::optional<uint32_t>;
        index_t graphics_queue_index;
        index_t present_queue_index;
        index_t transfer_queue_index;

        queue_indices(queue_indices const& other) noexcept = default;
        queue_indices(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface)
        {
            const auto queue_families = physical_device.getQueueFamilyProperties();
            for (uint32_t index = 0; index < queue_families.size(); ++index) {
                if (!graphics_queue_index.has_value() &&
                    queue_families[index].queueFlags & vk::QueueFlagBits::eGraphics) {
                    graphics_queue_index = index;
                }

                if (!transfer_queue_index.has_value() &&
                    queue_families[index].queueFlags & vk::QueueFlagBits::eTransfer &&
                    index != graphics_queue_index.value_or(-1)) {
                    transfer_queue_index = index;
                }

                if (!present_queue_index.has_value() && physical_device.getSurfaceSupportKHR(index, surface)) {
                    present_queue_index = index;
                }

                if (is_complete()) {
                    break;
                }
            }

            if (!transfer_queue_index.has_value()) {
                transfer_queue_index = graphics_queue_index;
            }
        }

        uint32_t
        queue_count() const
        {
            if (!is_complete())
                return 0;
            uint32_t queue_count = 1;
            if (present_queue_index.value() != graphics_queue_index.value()) {
                queue_count++;
            }
            if (transfer_queue_index.value() != graphics_queue_index.value() &&
                transfer_queue_index.value() != present_queue_index.value()) {
                queue_count++;
            }
            return queue_count;
        }

        uint32_t
        get_queue_index_0() const
        {
            return graphics_queue_index.value();
        }

        uint32_t
        get_queue_index_1() const
        {
            return present_queue_index != graphics_queue_index ? present_queue_index.value()
                                                               : transfer_queue_index.value();
        }

        uint32_t
        get_queue_index_2() const
        {
            return transfer_queue_index.value();
        }

        bool
        is_complete() const noexcept
        {
            return graphics_queue_index.has_value() && present_queue_index.has_value() &&
                   transfer_queue_index.has_value();
        }
    };

    export struct device : vk::Device {
        vk::PhysicalDevice physical_device;
        VmaAllocator allocator;
        uint32_t graphics_queue_index;
        uint32_t present_queue_index;
        uint32_t transfer_queue_index;

    private:
        static vk::Device
        create_logical_device(vk::PhysicalDevice physical_device, queue_indices indices)
        {
            static constexpr float priority = 1.0f;

            vk::PhysicalDeviceFeatures required_features{};
            required_features.fillModeNonSolid = vk::True;
            required_features.wideLines = vk::True;

            std::array<vk::DeviceQueueCreateInfo, 3> queue_infos = { vk::DeviceQueueCreateInfo{
                                                                         {},
                                                                         indices.get_queue_index_0(),
                                                                         1,
                                                                         &priority,
                                                                     },
                                                                     vk::DeviceQueueCreateInfo{
                                                                         {},
                                                                         indices.get_queue_index_1(),
                                                                         1,
                                                                         &priority,
                                                                     },
                                                                     vk::DeviceQueueCreateInfo{
                                                                         {},
                                                                         indices.get_queue_index_2(),
                                                                         1,
                                                                         &priority,
                                                                     } };

            constexpr auto extensions = required_device_extensions();
            constexpr uint32_t extension_count = static_cast<uint32_t>(extensions.size());

            if constexpr (enable_debug_validation()) {
                const char* debug_layer_name = "VK_LAYER_KHRONOS_validation";
                vk::DeviceCreateInfo create_info{ {},
                                                  indices.queue_count(),
                                                  queue_infos.data(),
                                                  1,
                                                  &debug_layer_name,
                                                  extension_count,
                                                  extensions.data(),
                                                  &required_features

                };

                return physical_device.createDevice(create_info);
            } else {
                vk::DeviceCreateInfo create_info{ {},      indices.queue_count(), queue_infos.data(), 0,
                                                  nullptr, extension_count,       extensions.data(),  &required_features

                };
                return physical_device.createDevice(create_info);
            }
        }

        static VmaAllocator
        create_allocator(vk::Device device, vk::PhysicalDevice physical_device)
        {
            VmaAllocatorCreateInfo create_info{};
            create_info.vulkanApiVersion = obscure::vulkan_version();
            create_info.instance = get_application_instance();
            create_info.physicalDevice = physical_device;
            create_info.device = device;

            VmaAllocator result{};
            vmaCreateAllocator(&create_info, &result);
            return result;
        }

        device(vk::PhysicalDevice physical_device, queue_indices indices)
            : vk::Device(create_logical_device(physical_device, indices))
            , physical_device(physical_device)
            , allocator(create_allocator(get_device(), physical_device))
            , graphics_queue_index(indices.graphics_queue_index.value())
            , present_queue_index(indices.present_queue_index.value())
            , transfer_queue_index(indices.transfer_queue_index.value())
        {
        }

    public:
        device(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface)
            : device(physical_device, queue_indices{ physical_device, surface })
        {
        }

        device(vk::SurfaceKHR surface, std::function<float(vk::PhysicalDevice)> get_device_score)
            : device(pick_device(surface, std::move(get_device_score)), surface)
        {
        }

        device(device&& other) noexcept
            : vk::Device(other.get_device())
            , physical_device(other.physical_device)
            , graphics_queue_index(other.graphics_queue_index)
            , present_queue_index(other.present_queue_index)
        {
            static_cast<vk::Device&>(other) = VK_NULL_HANDLE;
        }

        [[nodiscard]] vk::Device
        get_device() const noexcept
        {
            return static_cast<vk::Device>(*this);
        }

        [[nodiscard]] VmaAllocator
        get_vma_allocator() const noexcept
        {
            return allocator;
        }

        [[nodiscard]] vk::PhysicalDevice
        get_physical_device() const noexcept
        {
            return physical_device;
        }

        [[nodiscard]] uint32_t
        get_graphics_queue_index() const noexcept
        {
            return graphics_queue_index;
        }

        [[nodiscard]] uint32_t
        get_present_queue_index() const noexcept
        {
            return present_queue_index;
        }

        [[nodiscard]] uint32_t
        get_transfer_queue_index() const noexcept
        {
            return transfer_queue_index;
        }

        [[nodiscard]] vk::Queue
        get_graphics_queue() const
        {
            return get_device().getQueue(graphics_queue_index, 0);
        }

        [[nodiscard]] vk::Queue
        get_present_queue() const
        {
            return get_device().getQueue(present_queue_index, 0);
        }

        [[nodiscard]] vk::Queue
        get_transfer_queue() const
        {
            return get_device().getQueue(transfer_queue_index, 0);
        }

        ~device()
        {
            if (get_device() != VK_NULL_HANDLE) {
                get_device().waitIdle();
                vmaDestroyAllocator(allocator);
                destroy();
            }
        }
    };
} // namespace obscure::vulkan
