module;
#include <array>
#include <exception>
#include <format>
#include <print>
#include <source_location>
export module obscure.utils.check_result;

template<std::array Vals>
bool contains(auto val) noexcept
{
    for (auto const& elem : Vals) {
        if (elem == val) {
            return true;
        }
    }
    return false;
}

export namespace obscure
{
    template<std::array OK, std::array Warn>
    void check_result(auto value, std::source_location location = std::source_location::current())
    {
        if (contains<OK>(value)) {
            return;
        }else if (contains<Warn>(value)) {
            std::println("WARN: '{}'\n\temitted from '{}' at '{}:{}:{}'", value, location.function_name(), location.file_name(), location.line(), location.column());
            return;
        }else {
            std::string error_message = std::format("ERR: '{}'\n\temitted from '{}' at '{}:{}:{}'", value, location.function_name(), location.file_name(), location.line(), location.column());
            std::println("{}", error_message);
            throw std::runtime_error{error_message};
        }
    }
}