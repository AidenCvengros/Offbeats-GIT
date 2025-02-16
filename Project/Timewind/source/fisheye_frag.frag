#version 450

layout(set= 0, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
  float z = sqrt(1.0 - fragTexCoord.x * fragTexCoord.x - fragTexCoord.y * fragTexCoord.y);
  float a = 1.0 / (z * tan(100 * 0.5));
  outColor = texture(texSampler, (fragTexCoord - 0.5) * 2.0 * a) * fragColor;
}