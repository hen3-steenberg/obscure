#include <bit>
#include <utility>
#ifndef OBSCURE_VULKAN_LOGGER_DEFINITION
#define OBSCURE_VULKAN_LOGGER_DEFINITION 1
#include "glfw_vulkan_include.hpp"
#include "obscure/helper_templates/parent_reference.hpp"
#include "obscure/helper_templates/max_set_bit.hpp"
#include <iostream>
#include <fstream>
#include <atomic>


namespace obscure
{
    namespace vulkan
    {
        template<typename ... TSiblings>
        struct logger_base : private obscure::helper_templates::parent_ref<vk::Instance, TSiblings...>
        {
            private:
            static VKAPI_ATTR VkBool32 VKAPI_CALL LogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                                void* pUserData) 
            {
                if(pUserData)
                {
                    logger_base* logger = reinterpret_cast<logger_base*>(pUserData);
                    logger->LogEvent(messageSeverity, messageType, pCallbackData);
                }
                return VK_FALSE;
            }

            vk::DebugUtilsMessengerEXT LoggerHandle;

            public:

            logger_base()
            {
                vk::DebugUtilsMessengerCreateInfoEXT create_info {
                        {},
                        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding | vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                        LogCallback,
                        this
                    };

                LoggerHandle = obscure::helper_templates::parent_ref<vk::Instance, TSiblings...>::get_parent_ref().createDebugUtilsMessengerEXT(create_info);
            }

            ~logger_base()
            {
                obscure::helper_templates::parent_ref<vk::Instance, TSiblings...>::get_parent_ref().destroyDebugUtilsMessengerEXT(LoggerHandle);
            }

            protected:

            virtual void LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT* Data) = 0;


        };

        constexpr const char* get_message_severity_text(VkDebugUtilsMessageSeverityFlagBitsEXT Severity) noexcept
        {
            using namespace obscure::helper_templates;
            constexpr auto verbose = max_set_bits(VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT);
            switch(max_set_bits(Severity))
            {
                case max_set_bits(VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT):
                    return "VERBOSE";
                case max_set_bits(VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT):
                    return "   INFO";
                case max_set_bits(VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT):
                    return "WARNING";
                case max_set_bits(VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT):
                    return "  ERROR";
                default:
                    return "UNKNOWN";
            }
        }

        constexpr const char* get_message_type_text(VkDebugUtilsMessageTypeFlagsEXT Type) noexcept
        {
            using namespace obscure::helper_templates;
            switch (max_set_bits(Type)) 
            {
                case max_set_bits(VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT):
                    return "    GENERAL";
                case max_set_bits(VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT):
                    return " VALIDATION";
                case max_set_bits(VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT):
                    return "PERFORMANCE";
                case max_set_bits(VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT):
                    return "ADDRES BIND";
                
                default:
                    return "    UNKNOWN";
            }
        }

        template<typename ... TSiblings>
        struct verbose_console_logger : logger_base<TSiblings ...>
        {
            private:
            void LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT* Data) final
            {
                std::cout << get_message_severity_text(Severity) << ", " << get_message_type_text(Type) << " : " << Data->pMessage << "\n";
            }
        };

        template<typename ... TSiblings>
        struct verbose_json_logger : logger_base<TSiblings ...>
        {
            std::ofstream json_file;
            std::atomic_flag is_busy = ATOMIC_FLAG_INIT;
            verbose_json_logger()
                : json_file("vulkan_log.json")
            {
                json_file << "[\n";
            }


            ~verbose_json_logger()
            {
                is_busy.wait(true);
                json_file << "\n]";
                json_file.flush();
                json_file.close();
            }

            private:
            void LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT* Data) final
            {
                is_busy.test_and_set();
                json_file << "{\n";
                json_file << "\t\"severity\" : \"" << get_message_severity_text(Severity) << "\",\n";
                json_file << "\t\"type\" : \"" << get_message_type_text(Type) << "\",\n";
                json_file << "\t\"id\" : " << Data->messageIdNumber << ",\n";
                json_file << "\t\"id_name\" : \"" << Data->pMessageIdName << "\",\n";
                json_file << "\t\"message\" : \"" << Data->pMessage << "\",\n";
                json_file << "\t\"queue_labels\" : [\n";
                for(size_t label_idx = 0; label_idx < Data->queueLabelCount; ++label_idx)
                {
                    const auto& label = Data->pQueueLabels[label_idx];
                    json_file << "\t{\n";
                    json_file << "\t\t\"name\" : \"" << label.pLabelName << "\",\n";
                    json_file << "\t\t\"color\" : [" << label.color[0] << ", " << label.color[1] << ", " << label.color[2] << ", " << label.color[3] << "]\n";
                    json_file << "\t},\n";
                }
                json_file << "\t],\n";
                json_file << "\t\"command_buffer_labels\" : [\n";
                for(size_t label_idx = 0; label_idx < Data->cmdBufLabelCount; ++label_idx)
                {
                    const auto& label = Data->pCmdBufLabels[label_idx];
                    json_file << "\t{\n";
                    json_file << "\t\t\"name\" : \"" << label.pLabelName << "\",\n";
                    json_file << "\t\t\"color\" : [" << label.color[0] << ", " << label.color[1] << ", " << label.color[2] << ", " << label.color[3] << "]\n";
                    json_file << "\t},\n";
                }
                json_file << "\t],\n";
                json_file << "\t\"objects\" : [\n";
                for(size_t obj_idx = 0; obj_idx < Data->objectCount; ++obj_idx)
                {
                    const auto& obj = Data->pObjects[obj_idx];
                    json_file << "\t{\n";
                    json_file << "\t\t\"type\" : " << obj.objectType << ",\n";
                    json_file << "\t\t\"handle\" : " << obj.objectHandle << ",\n";
                    json_file << "\t\t\"name\" : \"" << obj.pObjectName << "\"\n";
                    json_file << "\t},\n";
                }
                json_file << "\t]\n";
                json_file << "},\n";
                is_busy.clear();
            }
        };

        template<typename TSibling,template<typename ... TSiblings> class TLogger1, template<typename ... TSiblings> class ... TLoggers>
        struct logger_collection_impl : TLogger1<TSibling>, logger_collection_impl<std::pair<TSibling, TLogger1<TSibling>>, TLoggers...>
        {

        };

        template<typename TSibling,template<typename ... TSiblings> class TLogger1>
        struct logger_collection_impl<TSibling, TLogger1> : TLogger1<TSibling>
        {

        };

        template<bool Enable, typename TSibling, template<typename ... TSiblings> class ... TLoggers>
        struct logger_collection_s : logger_collection_impl<TSibling, TLoggers...>
        {

        };

        template<typename TSibling, template<typename ... TSiblings> class ... TLoggers>
        struct logger_collection_s<false, TSibling, TLoggers...>
        {};

        template<bool Enable, template<typename ... TSiblings> class TLogger1, template<typename ... TSiblings> class ... TLoggers>
        struct logger_collection : TLogger1<>, logger_collection_impl<TLogger1<>, TLoggers...>
        {

        };

        template<template<typename ... TSiblings> class TLogger1, template<typename ... TSiblings> class ... TLoggers>
        struct logger_collection<false, TLogger1, TLoggers...>
        {};
    }
}

#endif