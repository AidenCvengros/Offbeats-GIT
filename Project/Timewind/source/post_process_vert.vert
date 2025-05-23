#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main()
{
  vec2 modPosition = inPosition * 4.0;
  modPosition.x = modPosition.x - 1.0;
  modPosition.y = modPosition.y + 1.0;
  //modPosition.x = modPosition.x * -1;
  
  gl_Position = vec4(modPosition, 0.0, 1.0);
  fragColor = inColor;
  fragTexCoord = inTexCoord;
}