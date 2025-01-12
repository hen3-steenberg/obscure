#ifndef OBSCURE_VULKAN_INSTANCE_DEFINITION
#define OBSCURE_VULKAN_INSTANCE_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/obscure_properties.hpp"
#include "obscure/utils/name_list.hpp"
#include <iostream>
#include <atomic>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace obscure
{
    namespace vulkan
    {

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
            static std::atomic_flag is_vulkan_initialized = ATOMIC_FLAG_INIT;
            
            const vk::ApplicationInfo app_info {
                app_name,
                app_version.vulkan_version(),
                obscure_name(),
                obscure_version().vulkan_version(),
                VK_API_VERSION_1_3
		    };

            const auto extension_list = get_required_extensions();

            if constexpr (enable_debug_validation())
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

                if(!is_vulkan_initialized.test_and_set())
                {      
                    VULKAN_HPP_DEFAULT_DISPATCHER.init();
                    auto res = vk::createInstance(create_info.get());
                    VULKAN_HPP_DEFAULT_DISPATCHER.init(res);
                    return res;
                }
                else
                {
                    return vk::createInstance(create_info.get());
                }
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

                
                if(!is_vulkan_initialized.test_and_set())
                {      
                    VULKAN_HPP_DEFAULT_DISPATCHER.init();
                    auto res = vk::createInstance(create_info);
                    VULKAN_HPP_DEFAULT_DISPATCHER.init(res);
                    return res;
                }
                else
                {
                    return vk::createInstance(create_info);
                }
            }
        }

        struct instance : vk::Instance
        {
            instance(const char * app_name, obscure::version app_version)
                : vk::Instance(create_instance(app_name, app_version))
            {

            }

            instance(vk::Instance other)
                : vk::Instance(other)
            {
            }

            [[nodiscard]] vk::Instance get() const noexcept
            {
                return *this;
            }

            instance(instance const& other) = delete;

            ~instance() noexcept
            {
                destroy();
            }
        };
    }
}
#endif