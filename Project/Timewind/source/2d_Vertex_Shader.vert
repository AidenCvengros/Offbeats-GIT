#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject
{
  mat4 model;
  mat4 view;
  mat4 proj;
  vec4 lookAt;
  vec4 camPos;
  //vec4 playerPos;
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
  
  vec4 vertWorldPosition = ps.model * vec4(inPosition, 0.0, 1.0);
  
  vec3 screenCenter = vec3(ubo.camPos.x + ubo.lookAt.x, ubo.camPos.y + ubo.lookAt.y, 0.0);
  vec3 camVec = vertWorldPosition.xyz - screenCenter;
  float distanceToCamera = sqrt(camVec.x * camVec.x);
  
  vertWorldPosition.z += 10.0f - (distanceToCamera * distanceToCamera) / 14.0f;
  //if (distanceToCamera <= 2.0f)
  //{
  //  vertWorldPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  //  }
  gl_Position = ubo.proj * ubo.view * vertWorldPosition;
  fragColor = ps.color;
  fragTexCoord = vec2(inTexCoord.x, 1.0f - inTexCoord.y);
}