#version 450
layout(push_constant, std430) uniform model {
    mat4 transform;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = transform * vec4(inPosition, 1.0);
    fragColor = inColor;
}