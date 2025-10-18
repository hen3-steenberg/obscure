module;
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.application_context;
export import obscure.utils.version;
import obscure.properties;
import obscure.utils.name_list;
import obscure.helper_templates.max_set_bit;

export namespace obscure
{
    void
    initialize(const char* app_name, obscure::version app_version);
    vk::Instance
    get_application_instance();
    void
    free_instance() noexcept;
} // namespace obscure