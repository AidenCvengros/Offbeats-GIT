#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main()
{
  //vec4 vVertex = ubo.view * tempPos;
  //
  //float dist = length(vVertex);
  //vec4 proj = dot(tempPos, normalize(ubo.lookAt)) * normalize(ubo.lookAt);
  //vec4 c = tempPos - proj;
  //
  //float magnitude = 1 - acos(dot(normalize(vVertex), normalize(tempPos)));
  //
  //c = length(c) * magnitude * normalize(c);
  //vec4 dir2 = normalize(c - ubo.lookAt);
  //dir2 = (dir2 * dist);
  
  gl_Position = vec4(inPosition, 0.0, 1.0);
  fragColor = inColor;
  fragTexCoord = inTexCoord;
}