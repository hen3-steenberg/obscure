module;

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.hpp>

export module obscure.vulkan.dispatch.instance;

export namespace obscure::vulkan::dispatch {
    struct instance {
        vk::Instance vk_instance;
        vk::detail::DispatchLoaderDynamic vk_dispatch_loader;

        instance(vk::InstanceCreateInfo const&);
        ~instance();

        vk::DebugReportCallbackEXT createDebugReportCallbackEXT(vk::DebugReportCallbackCreateInfoEXT const&, vk::AllocationCallbacks const&) const&;
        vk::DebugReportCallbackEXT createDebugReportCallbackEXT(vk::DebugReportCallbackCreateInfoEXT const&) const&;

        vk::DebugUtilsMessengerEXT createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT const&, vk::AllocationCallbacks const&) const&;
        vk::DebugUtilsMessengerEXT createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT const&) const&;

        vk::SurfaceKHR createDisplayPlaneSurfaceKHR(vk::DisplaySurfaceCreateInfoKHR const&, vk::AllocationCallbacks const&) const&;
        vk::SurfaceKHR createDisplayPlaneSurfaceKHR(vk::DisplaySurfaceCreateInfoKHR const&) const&;

        vk::SurfaceKHR createHeadlessSurfaceEXT(vk::HeadlessSurfaceCreateInfoEXT const&, vk::AllocationCallbacks const&) const&;
        vk::SurfaceKHR createHeadlessSurfaceEXT(vk::HeadlessSurfaceCreateInfoEXT const&) const&;

        void debugReportMessageEXT(vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT type, uint64_t object, size_t location, int32_t code, const char * pLayerPrefix, const char * pMessage) const&;

        void destroyDebugReportCallbackEXT(vk::DebugReportCallbackEXT, vk::AllocationCallbacks const&) const&;
        void destroyDebugReportCallbackEXT(vk::DebugReportCallbackEXT) const&;

        void destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT, vk::AllocationCallbacks const&) const&;
        void destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT) const&;

        void destroySurfaceKHR(vk::SurfaceKHR, vk::AllocationCallbacks const&) const&;
        void destroySurfaceKHR(vk::SurfaceKHR) const&;

        std::vector<vk::PhysicalDeviceGroupProperties> enumeratePhysicalDeviceGroups() const&;

        std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const&;

        PFN_vkVoidFunction getProcAddr(const char * pName) const&;

        void submitDebugUtilsMessageEXT(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, vk::DebugUtilsMessengerCallbackDataEXT const* pCallBackData) const&;

    };
}
