#version 450

layout(set = 0, binding = 0) uniform FisheyeUniformBufferObject
{
  float fisheyeStrength;
  float screenWidth;
  float screenHeight;
} fubo;

layout(set= 0, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

vec2 res = vec2(320, 240);
const vec3 defaultColor = vec3(0.025, 0.0125, 0.0);
// Amount of shadow mask.
float maskDark=0.5;
float maskLight=1.5;

// Nearest emulated sample given floating point position and texel offset.
// Also zero's off screen.
vec3 Fetch(vec2 pos,vec2 off)
{
  pos=floor(pos*res+off)/res;
  if(max(abs(pos.x-0.5),abs(pos.y-0.5))>0.5)return defaultColor;
  return texture(texSampler,pos.xy,-16.0).rgb;
}

// Distance in emulated pixels to nearest texel.
vec2 Dist(vec2 pos)
{
  pos=pos*res;
  return -((pos-floor(pos))-vec2(0.5));
}

// 1D Gaussian.
float Gaus(float pos,float scale)
{
  return exp2(scale*pos*pos);
}

// 3-tap Gaussian filter along horz line.
vec3 Horz3(vec2 pos,float off)
{
  vec3 b=Fetch(pos,vec2(-1.0,off));
  vec3 c=Fetch(pos,vec2( 0.0,off));
  vec3 d=Fetch(pos,vec2( 1.0,off));
  float dst=Dist(pos).x;
  // Convert distance to weight.
  float scale=-3.0;
  float wb=Gaus(dst-1.0,scale);
  float wc=Gaus(dst+0.0,scale);
  float wd=Gaus(dst+1.0,scale);
  // Return filtered sample.
  return (b*wb+c*wc+d*wd)/(wb+wc+wd);
}

// 5-tap Gaussian filter along horz line.
vec3 Horz5(vec2 pos,float off)
{
  vec3 a=Fetch(pos,vec2(-2.0,off));
  vec3 b=Fetch(pos,vec2(-1.0,off));
  vec3 c=Fetch(pos,vec2( 0.0,off));
  vec3 d=Fetch(pos,vec2( 1.0,off));
  vec3 e=Fetch(pos,vec2( 2.0,off));
  float dst=Dist(pos).x;
  // Convert distance to weight.
  float scale=-3.0;
  float wa=Gaus(dst-2.0,scale);
  float wb=Gaus(dst-1.0,scale);
  float wc=Gaus(dst+0.0,scale);
  float wd=Gaus(dst+1.0,scale);
  float we=Gaus(dst+2.0,scale);
  // Return filtered sample.
  return (a*wa+b*wb+c*wc+d*wd+e*we)/(wa+wb+wc+wd+we);
}

// Return scanline weight.
float Scan(vec2 pos,float off)
{
  float dst=Dist(pos).y;
  return Gaus(dst+off,-8.0);
}

// Allow nearest three lines to effect pixel.
vec3 Tri(vec2 pos)
{
  vec3 a=Horz3(pos,-1.0);
  vec3 b=Horz5(pos, 0.0);
  vec3 c=Horz3(pos, 1.0);
  float wa=Scan(pos,-1.0);
  float wb=Scan(pos, 0.0);
  float wc=Scan(pos, 1.0);
  return a*wa+b*wb+c*wc;
}

// Shadow mask.
vec3 Mask(vec2 pos)
{
  pos.x+=pos.y*3.0;
  vec3 mask=vec3(maskDark,maskDark,maskDark);
  pos.x=fract(pos.x/6.0);
  if(pos.x<0.333)mask.r=maskLight;
  else if(pos.x<0.666)mask.g=maskLight;
  else mask.b=maskLight;
  return mask;
}

void main()
{
  

  vec2 halfVec = vec2(0.5, 0.5);
  vec2 fragVec = (fragTexCoord * 2.0) - halfVec;
  float cornerDist = dot(halfVec, halfVec);
  float power = ( 3.141592 / sqrt(cornerDist)) * fubo.fisheyeStrength;
  vec2 newTexCoords = normalize(fragVec) * tan(sqrt(dot(fragVec, fragVec) * power)) * cornerDist / tan(cornerDist * power);
  
  // Calculates the position so the screen is always 4:3
  float ratio = (fubo.screenWidth / fubo.screenHeight) / (4.0 / 3.0);
  if (ratio > 1.0)
  {
    newTexCoords.x = newTexCoords.x * ratio;
  }
  else
  {
    newTexCoords.y = newTexCoords.y / ratio;
  }
  
  newTexCoords += halfVec;
  
  // Colors outside the "screen" brown
  if (newTexCoords.x > 0.9875 || newTexCoords.x < 0.0125 || newTexCoords.y > 0.9875 || newTexCoords.y < 0.0125)
  {
    outColor = vec4(defaultColor, 1.0);
  }
  else
  {
    vec3 newColor = Tri(newTexCoords)*Mask(newTexCoords * res * 6.0);
    outColor = vec4(newColor, 1.0);
  }
}