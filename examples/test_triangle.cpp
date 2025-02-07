#include "obscure/vulkan/builtin_pipelines/test_triangle.hpp"
#include "obscure/graphics_context.hpp"
#include "obscure/application_context.hpp"

int main()
{
    obscure::initialize("Triangle Test App", obscure::version{1,0,0});

    obscure::graphics_context<obscure::vulkan::builtin_pipelines::test_triangle> app{};

    while (!app.window.should_close())
    {
        glfwPollEvents();
        if (app.window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            break;
        }
        {
            auto frame_ctx = app.begin_frame();
            frame_ctx.draw_test_triangle();
        }
        app.submit_frame();
        app.draw_frame();
    }

    return 0;
}