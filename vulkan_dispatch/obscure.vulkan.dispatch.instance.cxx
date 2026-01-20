module;

#include <vulkan/vulkan.hpp>

module obscure.vulkan.dispatch.instance;
import obscure.vulkan.result;

namespace obscure::vulkan::dispatch {

    instance::instance(vk::InstanceCreateInfo const& create_info) {
        vk_dispatch_loader.init();
        obscure::vulkan::check(vk::createInstance(&create_info, nullptr, &vk_instance, vk_dispatch_loader));
        vk_dispatch_loader.init(vk_instance);
    }


    instance::~instance() {
        if(vk_instance != VK_NULL_HANDLE) {
            vk_instance.destroy(nullptr, vk_dispatch_loader);
        }
    }



    vk::DebugReportCallbackEXT instance::createDebugReportCallbackEXT(vk::DebugReportCallbackCreateInfoEXT const& create_info, vk::AllocationCallbacks const& allocator) const& {
        vk::DebugReportCallbackEXT callback{};
        obscure::vulkan::check(vk_instance.createDebugReportCallbackEXT(&create_info, &allocator, &callback, vk_dispatch_loader));
        return callback;
    }
    vk::DebugReportCallbackEXT instance::createDebugReportCallbackEXT(vk::DebugReportCallbackCreateInfoEXT const& create_info) const& {
        vk::DebugReportCallbackEXT callback{};
        obscure::vulkan::check(vk_instance.createDebugReportCallbackEXT(&create_info, nullptr, &callback, vk_dispatch_loader));
        return callback;
    }

    vk::DebugUtilsMessengerEXT instance::createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT const& create_info, vk::AllocationCallbacks const& allocator) const& {
        vk::DebugUtilsMessengerEXT result{};
        obscure::vulkan::check(vk_instance.createDebugUtilsMessengerEXT(&create_info, &allocator, &result, vk_dispatch_loader));
        return result;
    }
    vk::DebugUtilsMessengerEXT instance::createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT const& create_info) const& {
        vk::DebugUtilsMessengerEXT result{};
        obscure::vulkan::check(vk_instance.createDebugUtilsMessengerEXT(&create_info, nullptr, &result, vk_dispatch_loader));
        return result;
    }

    vk::SurfaceKHR instance::createDisplayPlaneSurfaceKHR(vk::DisplaySurfaceCreateInfoKHR const& create_info, vk::AllocationCallbacks const& allocator) const& {
        vk::SurfaceKHR result{};
        obscure::vulkan::check(vk_instance.createDisplayPlaneSurfaceKHR(&create_info, &allocator, &result, vk_dispatch_loader));
        return result;
    }
    vk::SurfaceKHR instance::createDisplayPlaneSurfaceKHR(vk::DisplaySurfaceCreateInfoKHR const& create_info) const& {
        vk::SurfaceKHR result{};
        obscure::vulkan::check(vk_instance.createDisplayPlaneSurfaceKHR(&create_info, nullptr, &result, vk_dispatch_loader));
        return result;
    }

    vk::SurfaceKHR instance::createHeadlessSurfaceEXT(vk::HeadlessSurfaceCreateInfoEXT const& create_info, vk::AllocationCallbacks const& allocator) const& {
        vk::SurfaceKHR result{};
        obscure::vulkan::check(vk_instance.createHeadlessSurfaceEXT(&create_info, &allocator, &result, vk_dispatch_loader));
        return result;
    }
    vk::SurfaceKHR instance::createHeadlessSurfaceEXT(vk::HeadlessSurfaceCreateInfoEXT const& create_info) const& {
        vk::SurfaceKHR result{};
        obscure::vulkan::check(vk_instance.createHeadlessSurfaceEXT(&create_info, nullptr, &result, vk_dispatch_loader));
        return result;
    }

    void instance::debugReportMessageEXT(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT type, uint64_t object, size_t location, int32_t code, const char * pLayerPrefix, const char * pMessage) const& {
        vk_instance.debugReportMessageEXT(flags, type, object, location, code, pLayerPrefix, pMessage, vk_dispatch_loader);
    }

    void instance::destroyDebugReportCallbackEXT(vk::DebugReportCallbackEXT callback, vk::AllocationCallbacks const& allocator) const& {
        vk_instance.destroyDebugReportCallbackEXT(callback, &allocator, vk_dispatch_loader);
    }
    void instance::destroyDebugReportCallbackEXT(vk::DebugReportCallbackEXT callback) const& {
        vk_instance.destroyDebugReportCallbackEXT(callback, nullptr, vk_dispatch_loader);
    }

    void instance::destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT messenger, vk::AllocationCallbacks const& allocator) const& {
        vk_instance.destroyDebugUtilsMessengerEXT(messenger, &allocator, vk_dispatch_loader);
    }
    void instance::destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT messenger) const& {
        vk_instance.destroyDebugUtilsMessengerEXT(messenger, nullptr, vk_dispatch_loader);
    }

    void instance::destroySurfaceKHR(vk::SurfaceKHR surface, vk::AllocationCallbacks const& allocator) const& {
        vk_instance.destroySurfaceKHR(surface, &allocator, vk_dispatch_loader);
    }
    void instance::destroySurfaceKHR(vk::SurfaceKHR surface) const& {
        vk_instance.destroySurfaceKHR(surface, nullptr, vk_dispatch_loader);
    }

    std::vector<vk::PhysicalDeviceGroupProperties> instance::enumeratePhysicalDeviceGroups() const& {
        return vk_instance.enumeratePhysicalDeviceGroups(vk_dispatch_loader);
    }

    std::vector<vk::PhysicalDevice> instance::enumeratePhysicalDevices() const& {
        return vk_instance.enumeratePhysicalDevices(vk_dispatch_loader);
    }

    PFN_vkVoidFunction instance::getProcAddr(const char * pName) const& {
        vk_instance.getProcAddr(pName, vk_dispatch_loader);
    }

    void instance::submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, vk::DebugUtilsMessengerCallbackDataEXT const* pCallBackData) const& {
        vk_instance.submitDebugUtilsMessageEXT(severity, type, pCallBackData, vk_dispatch_loader);
    }
}
