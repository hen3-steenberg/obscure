#version 450
layout(push_constant, std430) uniform model
{
    mat4 transform;
};

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTextCoord;

layout(location = 0) out vec2 outTextCoord;

void
main()
{
    gl_Position = transform * vec4(inPosition, 0.0, 1.0);
    outTextCoord = inTextCoord;
}