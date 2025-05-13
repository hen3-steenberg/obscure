module;
#include <GLFW/glfw3.h>
export module obscure.glfw:defaults;

export namespace obscure::glfw
{
        [[nodiscard]] int get_fullscreen_height() noexcept;
        [[nodiscard]] int get_fullscreen_width() noexcept;

        [[nodiscard]] int get_default_window_height() noexcept;
        [[nodiscard]] int get_default_window_width() noexcept;
}

struct default_monitor
{
        const GLFWvidmode* mode;
        default_monitor()
        {

                glfwInit();
                GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
                mode = glfwGetVideoMode(PrimaryMonitor);
        }

        int get_fullscreen_height() const noexcept
        {
                return mode->height;
        }

        int get_fullscreen_width() const noexcept
        {
                return mode->width;
        }

        ~default_monitor() {
                glfwTerminate();
        }
};

const default_monitor monitor{};

[[nodiscard]] int obscure::glfw::get_fullscreen_height() noexcept
{
        return monitor.get_fullscreen_height();
}

[[nodiscard]] int obscure::glfw::get_fullscreen_width() noexcept
{
        return monitor.get_fullscreen_width();
}

[[nodiscard]] int obscure::glfw::get_default_window_height() noexcept
{
        static int default_height = get_fullscreen_height();
        return default_height;
}


[[nodiscard]] int obscure::glfw::get_default_window_width() noexcept
{
        static int default_width = get_fullscreen_width();
        return default_width;
}