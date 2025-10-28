module;
#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>
#include <tuple>
export module obscure.glfw:window;
import :defaults;
import obscure.properties;

export namespace obscure::glfw
{
    struct glfw_window_ref {
        GLFWwindow* window_ptr;

        bool
        should_close() const
        {
            return glfwWindowShouldClose(window_ptr);
        }

        bool
        isKeyPressed(int KeyCode) const
        {
            return glfwGetKey(window_ptr, KeyCode) & GLFW_PRESS;
        }

        std::pair<double, double>
        getCursorPos() const
        {
            std::pair<double, double> cursor_pos{};
            glfwGetCursorPos(window_ptr, &cursor_pos.first, &cursor_pos.second);
            return cursor_pos;
        }

        vk::SurfaceKHR
        create_surface(vk::Instance inst) const
        {
            VkSurfaceKHR result;
            auto Err = glfwCreateWindowSurface(inst, window_ptr, nullptr, &result);
            if (Err != VK_SUCCESS) {
                throw std::runtime_error("Unable to create vulkan surface.");
            }
            return vk::SurfaceKHR{ result };
        }

        vk::Extent2D
        get_window_extent() const&
        {
            int width, height;
            glfwGetFramebufferSize(window_ptr, &width, &height);
            return vk::Extent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        }
    };

    struct glfw_window : glfw_window_ref {
        glfw_window();
        glfw_window(const glfw_window& other) = delete;
        glfw_window(glfw_window&& other);
        [[nodiscard]] glfw_window_ref
        get_window_ref() const noexcept;
        ~glfw_window();
    };
} // namespace obscure::glfw

obscure::glfw::glfw_window::glfw_window()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
    if constexpr (enable_debug_validation()) {
        window_ptr = glfwCreateWindow(get_default_window_width(), get_default_window_height(), "", nullptr, nullptr);
    } else {
        window_ptr = glfwCreateWindow(
            get_default_window_width(), get_default_window_height(), "", glfwGetPrimaryMonitor(), nullptr);
    }
}

obscure::glfw::glfw_window::glfw_window(glfw_window&& other)
    : glfw_window_ref(other)
{
    other.window_ptr = nullptr;
}

[[nodiscard]] obscure::glfw::glfw_window_ref
obscure::glfw::glfw_window::get_window_ref() const noexcept
{
    return static_cast<obscure::glfw::glfw_window_ref>(*this);
}

obscure::glfw::glfw_window::~glfw_window()
{
    if (window_ptr) {
        glfwDestroyWindow(window_ptr);
    }
}