module;
#include <iostream>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <atomic>
#include <vector>
#include <cstdint>
#include <utility>
#include <string>
#include <array>
#include <queue>
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
export module obscure.vulkan.application_context;
export import obscure.utils.version;
import obscure.properties;
import obscure.utils.name_list;
import obscure.helper_templates.max_set_bit;

export namespace obscure
{
    void initialize(const char * app_name, obscure::version app_version);
    vk::Instance get_application_instance();
    void free_instance() noexcept;
}

//VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#pragma region logger
namespace obscure::vulkan
{
        template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity>
        consteval vk::DebugUtilsMessageSeverityFlagsEXT get_severity_flags()
        {
            vk::DebugUtilsMessageSeverityFlagsEXT result{};
            if constexpr (vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose >= min_severity)
            {
                result |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
            }
            if constexpr (vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo >= min_severity)
            {
                result |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
            }
            if constexpr (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning >= min_severity)
            {
                result |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
            }
            if constexpr (vk::DebugUtilsMessageSeverityFlagBitsEXT::eError >= min_severity)
            {
                result |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
            }
            return result;
        }

        template<vk::DebugUtilsMessageTypeFlagBitsEXT min_type>
        consteval vk::DebugUtilsMessageTypeFlagsEXT get_type_flags()
        {
            vk::DebugUtilsMessageTypeFlagsEXT result{};
            if constexpr (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral >= min_type)
            {
                result |= vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral;
            }
            if constexpr (vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation >= min_type)
            {
                result |= vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
            }
            if constexpr (vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance >= min_type)
            {
                result |= vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
            }
            return result;
        }

        template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity>
        constexpr bool is_valid_severity(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
        {
            constexpr uint32_t min_value = std::bit_cast<uint32_t>(min_severity);
            return severity >= min_value;
        }

        template<>
        constexpr bool is_valid_severity<vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose>(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
        {
            return true;
        }

        template<vk::DebugUtilsMessageTypeFlagBitsEXT min_type>
        constexpr bool is_valid_type(VkDebugUtilsMessageTypeFlagsEXT messageType)
        {
            constexpr uint32_t min_value = std::bit_cast<uint32_t>(min_type);
            return messageType >= min_value;
        }


        export template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity, vk::DebugUtilsMessageTypeFlagBitsEXT min_type>
        struct logger_base
        {
            private:
                static vk::Instance get_instance()
                {
                    return obscure::get_application_instance();
                }

            static VKAPI_ATTR VkBool32 VKAPI_CALL LogCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                                void* pUserData)
            {
                if(is_valid_severity<min_severity>(messageSeverity) && is_valid_type<min_type>(messageType) && pUserData)
                {
                    auto logger = static_cast<logger_base*>(pUserData);
                    logger->LogEvent(messageSeverity, messageType, *pCallbackData);
                }
                return VK_FALSE;
            }

            vk::DebugUtilsMessengerEXT LoggerHandle;

            public:

            logger_base()
            {
                vk::DebugUtilsMessengerCreateInfoEXT create_info {
                        {},
                        get_severity_flags<min_severity>(),
                        get_type_flags<min_type>(),
                        LogCallback,
                        this
                    };
                auto instance = get_instance();
                LoggerHandle = instance.createDebugUtilsMessengerEXT(create_info);
            }

            void free() noexcept {
                if (LoggerHandle != VK_NULL_HANDLE) {
                    auto instance = get_instance();
                    instance.destroyDebugUtilsMessengerEXT(LoggerHandle);
                    LoggerHandle = VK_NULL_HANDLE;
                }
            }

            virtual ~logger_base() noexcept
            {
                free();
            }

            protected:

            virtual void LogEvent(VkDebugUtilsMessageSeverityFlagsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT & Data) = 0;

        };

        constexpr const char* get_message_severity_text(VkDebugUtilsMessageSeverityFlagsEXT Severity) noexcept
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

        struct Event
        {
            VkDebugUtilsMessageSeverityFlagsEXT Severity;
            VkDebugUtilsMessageTypeFlagsEXT Type;
            struct LogData
            {

                struct Label
                {
                    std::string name;
                    std::array<float, 4> color;

                    Label(vk::DebugUtilsLabelEXT const& lbl)
                        : name(lbl.pLabelName ? lbl.pLabelName : "NO NAME"), color(lbl.color)
                    {}
                };

                struct Object
                {
                    vk::ObjectType Type;
                    uint64_t Handle;
                    std::string name;

                    Object(vk::DebugUtilsObjectNameInfoEXT const& obj)
                        : Type(obj.objectType), Handle(obj.objectHandle), name(obj.pObjectName ? obj.pObjectName : "NO NAME")
                    {}
                };

                int32_t ID;
                std::string IDName;
                std::string Message;

                std::vector<Label> queue_labels;
                std::vector<Label> cmd_buffer_labels;

                std::vector<Object> objects;

                LogData(const VkDebugUtilsMessengerCallbackDataEXT & Data)
                    :   ID(Data.messageIdNumber),
                        IDName(Data.pMessageIdName ? Data.pMessageIdName : "NO NAME"),
                        Message(Data.pMessage ? Data.pMessage : "NO MESSAGE"),
                        queue_labels(),
                        cmd_buffer_labels(),
                        objects()
                {
                    queue_labels.reserve(Data.queueLabelCount);
                    for(size_t index = 0; index < Data.queueLabelCount; ++index)
                    {
                        queue_labels.emplace_back(Data.pQueueLabels[index]);
                    }

                    cmd_buffer_labels.reserve(Data.cmdBufLabelCount);
                    for(size_t index = 0; index < Data.cmdBufLabelCount; ++index)
                    {
                        cmd_buffer_labels.emplace_back(Data.pCmdBufLabels[index]);
                    }

                    objects.reserve(Data.objectCount);
                    for(size_t index = 0; index < Data.objectCount; ++index)
                    {
                        objects.emplace_back(Data.pObjects[index]);
                    }

                    for(auto & c : Message)
                    {
                        if(c == '"')
                        {
                            c = '\'';
                        }
                    }
                }
            } data;
            Event(VkDebugUtilsMessageSeverityFlagsEXT _Severity, VkDebugUtilsMessageTypeFlagsEXT _Type, const VkDebugUtilsMessengerCallbackDataEXT & _Data)
                : Severity(_Severity), Type(_Type), data(_Data)
            {

            }
        };

        struct EventQueue
        {
            std::queue<std::unique_ptr<Event>> events;
            std::mutex queue_mtx;

            void enqueue(VkDebugUtilsMessageSeverityFlagsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT & Data)
            {
                std::lock_guard queue_guard{queue_mtx};
                events.push(std::make_unique<Event>(Severity, Type, Data));
            }

            bool empty() const
            {
                return events.empty();
            }

            std::optional<std::unique_ptr<Event>> get_next_event()
            {
                std::lock_guard queue_guard{queue_mtx};
                if(events.empty()) return std::nullopt;
                else
                {
                    std::unique_ptr<Event> result = std::move(events.front());
                    events.pop();
                    return result;
                };
            }
        };

        export template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity, vk::DebugUtilsMessageTypeFlagBitsEXT min_type>
        struct console_logger : logger_base<min_severity, min_type>
        {
        private:
            void LogEvent(VkDebugUtilsMessageSeverityFlagsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT& Data) final
            {
                std::cout << get_message_severity_text(Severity) << ", " << get_message_type_text(Type) << " : " << Data.pMessage << "\n";
            }
        };

        export using verbose_console_logger = console_logger<vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo, vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral>;

        export template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity, vk::DebugUtilsMessageTypeFlagBitsEXT min_type>
        struct json_logger : logger_base<min_severity, min_type>
        {
            std::ofstream json_file;
            std::atomic_flag request_stop = ATOMIC_FLAG_INIT;
            EventQueue event_queue;
            std::thread writer_thread;
            bool first_entry = true;

            json_logger()
                : json_file("vulkan_log.json"), writer_thread([this]() { ProcessMessages(); })
            {
                json_file << "[\n";
            }


            ~json_logger()
            {
                request_stop.test_and_set();
                writer_thread.join();
                json_file << "\n]";
                json_file.flush();
                json_file.close();
            }

        private:

            void ProcessMessages()
            {
                using namespace std::literals;
                while (!request_stop.test() || !event_queue.empty())
                {
                    auto event = event_queue.get_next_event();
                    if (event.has_value())
                    {
                        HandleLogEntry(std::move(event.value()));
                    }
                    else
                    {
                        std::this_thread::sleep_for(100ms);
                    }
                }
            }

            void HandleLogEntry(std::unique_ptr<Event> Entry)
            {
                if (first_entry)
                {
                    json_file << "{\n";
                    first_entry = false;
                }
                else {
                    json_file << ",{\n";
                }

                json_file << "\t\"severity\" : \"" << get_message_severity_text(Entry->Severity) << "\",\n";
                json_file << "\t\"type\" : \"" << get_message_type_text(Entry->Type) << "\",\n";
                json_file << "\t\"id\" : " << Entry->data.ID << ",\n";
                json_file << "\t\"id_name\" : \"" << Entry->data.IDName << "\",\n";
                json_file << "\t\"message\" : \"" << Entry->data.Message << "\",\n";
                json_file << "\t\"queue_labels\" : [\n";

                bool first = true;

                for (auto const& label : Entry->data.queue_labels)
                {
                    if (first)
                    {
                        json_file << "\t{\n";
                        first = false;
                    }
                    else
                    {
                        json_file << "\t,{\n";
                    }
                    json_file << "\t\t\"name\" : \"" << label.name << "\",\n";
                    json_file << "\t\t\"color\" : [" << label.color[0] << ", " << label.color[1] << ", " << label.color[2] << ", " << label.color[3] << "]\n";
                    json_file << "\t}\n";
                }
                json_file << "\t],\n";
                json_file << "\t\"command_buffer_labels\" : [\n";

                first = true;

                for (auto const& label : Entry->data.cmd_buffer_labels)
                {
                    if (first)
                    {
                        json_file << "\t{\n";
                        first = false;
                    }
                    else
                    {
                        json_file << "\t,{\n";
                    }
                    json_file << "\t\t\"name\" : \"" << label.name << "\",\n";
                    json_file << "\t\t\"color\" : [" << label.color[0] << ", " << label.color[1] << ", " << label.color[2] << ", " << label.color[3] << "]\n";
                    json_file << "\t}\n";
                }
                json_file << "\t],\n";
                json_file << "\t\"objects\" : [\n";

                first = true;
                for (auto const& obj : Entry->data.objects)
                {
                    if (first)
                    {
                        json_file << "\t{\n";
                        first = false;
                    }
                    else
                    {
                        json_file << "\t,{\n";
                    }
                    json_file << "\t\t\"type\" : " << (size_t)obj.Type << ",\n";
                    json_file << "\t\t\"handle\" : " << obj.Handle << ",\n";
                    json_file << "\t\t\"name\" : \"" << obj.name << "\"\n";
                    json_file << "\t}\n";
                }
                json_file << "\t]\n";
                json_file << "}\n";
            }

            void LogEvent(VkDebugUtilsMessageSeverityFlagsEXT Severity, VkDebugUtilsMessageTypeFlagsEXT Type, const VkDebugUtilsMessengerCallbackDataEXT& Data) final
            {
                event_queue.enqueue(Severity, Type, Data);
            }
        };

        export using verbose_json_logger = json_logger< vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo, vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral>;

        export template<bool Enable, vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity, vk::DebugUtilsMessageTypeFlagBitsEXT min_type, template<vk::DebugUtilsMessageSeverityFlagBitsEXT, vk::DebugUtilsMessageTypeFlagBitsEXT> class ... TLoggers>
        struct logger_collection_impl : TLoggers<min_severity, min_type>...
        {

        };

        export template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity, vk::DebugUtilsMessageTypeFlagBitsEXT min_type, template<vk::DebugUtilsMessageSeverityFlagBitsEXT, vk::DebugUtilsMessageTypeFlagBitsEXT> class ... TLoggers>
        struct logger_collection_impl<false,min_severity, min_type, TLoggers...>
        {};

        export template<vk::DebugUtilsMessageSeverityFlagBitsEXT min_severity, vk::DebugUtilsMessageTypeFlagBitsEXT min_type, template<vk::DebugUtilsMessageSeverityFlagBitsEXT, vk::DebugUtilsMessageTypeFlagBitsEXT> class ... TLoggers>
        using logger_collection = logger_collection_impl<obscure::enable_debug_validation(), min_severity, min_type, TLoggers...>;
}
#pragma endregion



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
                &InstanceLifetimeCallback,
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

    [[nodiscard]] vk::Instance get_instance() const noexcept {
        return *this;
    }

    void free() const noexcept {
        if (get_instance() != VK_NULL_HANDLE) {
            destroy();
        }
    }

    ~ApplicationInstance() noexcept{
        free();
    }

};
    using logger_t =
        obscure::vulkan::logger_collection<
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral,
            obscure::vulkan::console_logger,
            obscure::vulkan::json_logger>;

std::optional<ApplicationInstance> ctx;
std::optional<logger_t> logger;

void obscure::initialize(const char * app_name, obscure::version app_version)
{
    if (!ctx.has_value()) {
        ctx.emplace(app_name, app_version);
        logger.emplace();
    }
}

vk::Instance obscure::get_application_instance()
{
    return ctx->get_instance();
}

void obscure::free_instance() noexcept {
    logger.reset();
    ctx.reset();
}