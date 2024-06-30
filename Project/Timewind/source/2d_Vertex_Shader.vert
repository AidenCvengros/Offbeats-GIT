#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform pushConstant
{
    layout(offset = 0) mat4 model;
    layout(offset = 64) vec4 color;
} ps;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main()
{
    gl_Position = ubo.proj * ubo.view * ps.model * vec4(inPosition, 0.0, 1.0);
    fragColor = ps.color;
    fragTexCoord = inTexCoord;
}