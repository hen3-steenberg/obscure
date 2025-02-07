#include "obscure/application_context.hpp"

#include "obscure/obscure_properties.hpp"
#include "obscure/utils/name_list.hpp"
#include "obscure/vulkan/logger.hpp"

#include <iostream>
#include <optional>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

obscure::name_list<obscure::enable_debug_validation()> get_required_extensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    obscure::name_list<obscure::enable_debug_validation()> result{glfwExtensions, glfwExtensionCount};
    if constexpr (obscure::enable_debug_validation())
    {
        result.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return result;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL InstanceLifetimeCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
}

vk::Instance create_instance(const char * app_name, obscure::version app_version)
{
    const vk::ApplicationInfo app_info {
        app_name,
        app_version.vulkan_version(),
        obscure::obscure_name(),
        obscure::obscure_version().vulkan_version(),
        VK_API_VERSION_1_0
    };

    const auto extension_list = get_required_extensions();
    if constexpr (obscure::enable_debug_validation())
    {
        const char* debug_layer_name = "VK_LAYER_KHRONOS_validation";
        vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> create_info
        {
            vk::InstanceCreateInfo {
                {},
                &app_info,
                1,
                &debug_layer_name,
                extension_list.get_count(),
                extension_list.get_names()
            },
            vk::DebugUtilsMessengerCreateInfoEXT {
                {},
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding | vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                InstanceLifetimeCallback,
                nullptr
            }
        };

        VULKAN_HPP_DEFAULT_DISPATCHER.init();
        auto res = vk::createInstance(create_info.get());
        VULKAN_HPP_DEFAULT_DISPATCHER.init(res);
        return res;
    }
    else
    {
        vk::InstanceCreateInfo create_info
        {
            {},
            &app_info,
            0,
            nullptr,
            extension_list.get_count(),
            extension_list.get_names()
        };

        VULKAN_HPP_DEFAULT_DISPATCHER.init();
        auto res = vk::createInstance(create_info);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(res);
        return res;
    }
}

struct ApplicationInstance : vk::Instance {
    ApplicationInstance(const char * app_name, obscure::version app_version)
        : vk::Instance(create_instance(app_name, app_version))
    {}

    ~ApplicationInstance() {
        destroy();
    }

};

struct ApplicationContext {
    ApplicationInstance instance;
    using logger_t =
        obscure::vulkan::logger_collection<
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral,
            obscure::vulkan::console_logger,
            obscure::vulkan::json_logger>;

    logger_t vk_loggers;

    ApplicationContext(const char * app_name, obscure::version app_version)
        : instance(app_name, app_version), vk_loggers() {

    }
};

std::optional<ApplicationContext> ctx;

void obscure::initialize(const char * app_name, obscure::version app_version)
{
    if (!ctx.has_value())
        ctx.emplace(app_name, app_version);
}

vk::Instance obscure::get_application_instance()
{
    return ctx->instance;
}
