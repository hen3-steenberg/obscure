module;
#include <array>
#include <format>
#include <source_location>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_to_string.hpp>
export module obscure.vulkan.result;
import obscure.utils.check_result;

export template<>
struct std::formatter<vk::Result, char> {

    template<class ParseContext>
    constexpr ParseContext::iterator
    parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        while (it != ctx.end()) {
            if (*it == '}') {
                return it;
            }
            ++it;
        }
        return ctx.end();
    }

    template<class FmtContext>
    constexpr FmtContext::iterator
    format(vk::Result const& res, FmtContext& ctx) const
    {
        std::string result_string = vk::to_string(res);

        return std::ranges::copy(result_string, ctx.out()).out;
    }
};

namespace obscure::vulkan
{

    template<vk::Result... Results>
    struct to_array;

    template<vk::Result R1, vk::Result... Results>
    struct to_array<R1, Results...> {
        constexpr static std::array value = std::array{ R1, Results... };
    };

    template<>
    struct to_array<> {
        constexpr static std::array<vk::Result, 0> value{};
    };

    export template<vk::Result... Warn>
    void
    check(vk::Result result, std::source_location location = std::source_location::current())
    {
        return obscure::check_result<std::array{ vk::Result::eSuccess }, to_array<Warn...>::value>(result, location);
    }

    export template<vk::Result... Warn>
    void
    check(VkResult result, std::source_location location = std::source_location::current())
    {
        return obscure::check_result<std::array{ vk::Result::eSuccess }, to_array<Warn...>::value>(vk::Result{ result },
                                                                                                   location);
    }
} // namespace obscure::vulkan
