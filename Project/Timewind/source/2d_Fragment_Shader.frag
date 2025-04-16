#version 450

layout(set= 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

const vec2 resolution = {800, 600};

//// Emulated input resolution.
//#if 0
//  // Fix resolution to set amount.
//  #define res (vec2(320.0/1.0,160.0/1.0))
//#else
//  // Optimize for resize.
//  #define res (resolution/6.0)
//#endif
//
//// Hardness of scanline.
////  -8.0 = soft
//// -16.0 = medium
//float hardScan=-8.0;
//
//// Hardness of pixels in scanline.
//// -2.0 = soft
//// -4.0 = hard
//float hardPix=-3.0;
//
//// Display warp.
//// 0.0 = none
//// 1.0/8.0 = extreme
//vec2 warp=vec2(1.0/32.0,1.0/24.0); 
//
//// Amount of shadow mask.
//float maskDark=0.5;
//float maskLight=1.5;
//
////------------------------------------------------------------------------
//
//// sRGB to Linear.
//// Assuing using sRGB typed textures this should not be needed.
//float ToLinear1(float c){return(c<=0.04045)?c/12.92:pow((c+0.055)/1.055,2.4);}
//vec3 ToLinear(vec3 c){return vec3(ToLinear1(c.r),ToLinear1(c.g),ToLinear1(c.b));}
//
//// Linear to sRGB.
//// Assuing using sRGB typed textures this should not be needed.
//float ToSrgb1(float c){return(c<0.0031308?c*12.92:1.055*pow(c,0.41666)-0.055);}
//vec3 ToSrgb(vec3 c){return vec3(ToSrgb1(c.r),ToSrgb1(c.g),ToSrgb1(c.b));}
//
//// Nearest emulated sample given floating point position and texel offset.
//// Also zero's off screen.
//vec3 Fetch(vec2 pos,vec2 off){
//  pos=floor(pos*res+off)/res;
//  if(max(abs(pos.x-0.5),abs(pos.y-0.5))>0.5)return vec3(0.0,0.0,0.0);
//  return ToLinear(texture(texSampler,pos.xy,-16.0).rgb);}
//
//// Distance in emulated pixels to nearest texel.
//vec2 Dist(vec2 pos){pos=pos*res;return -((pos-floor(pos))-vec2(0.5));}
//    
//// 1D Gaussian.
//float Gaus(float pos,float scale){return exp2(scale*pos*pos);}
//
//// 3-tap Gaussian filter along horz line.
//vec3 Horz3(vec2 pos,float off){
//  vec3 b=Fetch(pos,vec2(-1.0,off));
//  vec3 c=Fetch(pos,vec2( 0.0,off));
//  vec3 d=Fetch(pos,vec2( 1.0,off));
//  float dst=Dist(pos).x;
//  // Convert distance to weight.
//  float scale=hardPix;
//  float wb=Gaus(dst-1.0,scale);
//  float wc=Gaus(dst+0.0,scale);
//  float wd=Gaus(dst+1.0,scale);
//  // Return filtered sample.
//  return (b*wb+c*wc+d*wd)/(wb+wc+wd);}
//
//// 5-tap Gaussian filter along horz line.
//vec3 Horz5(vec2 pos,float off){
//  vec3 a=Fetch(pos,vec2(-2.0,off));
//  vec3 b=Fetch(pos,vec2(-1.0,off));
//  vec3 c=Fetch(pos,vec2( 0.0,off));
//  vec3 d=Fetch(pos,vec2( 1.0,off));
//  vec3 e=Fetch(pos,vec2( 2.0,off));
//  float dst=Dist(pos).x;
//  // Convert distance to weight.
//  float scale=hardPix;
//  float wa=Gaus(dst-2.0,scale);
//  float wb=Gaus(dst-1.0,scale);
//  float wc=Gaus(dst+0.0,scale);
//  float wd=Gaus(dst+1.0,scale);
//  float we=Gaus(dst+2.0,scale);
//  // Return filtered sample.
//  return (a*wa+b*wb+c*wc+d*wd+e*we)/(wa+wb+wc+wd+we);}
//
//// Return scanline weight.
//float Scan(vec2 pos,float off){
//  float dst=Dist(pos).y;
//  return Gaus(dst+off,hardScan);}
//
//// Allow nearest three lines to effect pixel.
//vec3 Tri(vec2 pos){
//  vec3 a=Horz3(pos,-1.0);
//  vec3 b=Horz5(pos, 0.0);
//  vec3 c=Horz3(pos, 1.0);
//  float wa=Scan(pos,-1.0);
//  float wb=Scan(pos, 0.0);
//  float wc=Scan(pos, 1.0);
//  return a*wa+b*wb+c*wc;}
//
//// Distortion of scanlines, and end of screen alpha.
//vec2 Warp(vec2 pos){
//  pos=pos*2.0-1.0;    
//  pos*=vec2(1.0+(pos.y*pos.y)*warp.x,1.0+(pos.x*pos.x)*warp.y);
//  return pos*0.5+0.5;}
//
//// Shadow mask.
//vec3 Mask(vec2 pos){
//  pos.x+=pos.y*3.0;
//  vec3 mask=vec3(maskDark,maskDark,maskDark);
//  pos.x=fract(pos.x/6.0);
//  if(pos.x<0.333)mask.r=maskLight;
//  else if(pos.x<0.666)mask.g=maskLight;
//  else mask.b=maskLight;
//  return mask;}    
//
//// Draw dividing bars.
//float Bar(float pos,float bar){pos-=bar;return pos*pos<4.0?0.0:1.0;}

vec2 curve(vec2 uv)
{
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;	
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
	uv  = (uv / 2.0) + 0.5;
	uv =  uv *0.92 + 0.04;
	return uv;
}

void main()
{
  outColor = texture(texSampler, fragTexCoord) * fragColor;
  
  //// Unmodified.
  //if(fragTexCoord.x<resolution.x*0.333){
  //  outColor.rgb=Fetch(fragTexCoord/resolution+vec2(0.333,0.0),vec2(0.0,0.0));}
  //else{
  //  vec2 pos=Warp(fragTexCoord.xy/resolution+vec2(-0.333,0.0));
  //  if(fragTexCoord.x<resolution.x*0.666){
  //    hardScan=-12.0;
  //    maskDark=maskLight=1.0;
  //    pos=Warp(fragTexCoord.xy/resolution);}
  //  outColor.rgb=Tri(pos)*Mask(fragTexCoord.xy);}    
  //outColor.a=1.0;  
  //outColor.rgb*=
  //  Bar(fragTexCoord.x,resolution.x*0.333)*
  //  Bar(fragTexCoord.x,resolution.x*0.666);
  //outColor.rgb=ToSrgb(outColor.rgb);
  
  //vec2 pos=Warp(fragTexCoord.xy/resolution);
  //outColor.rgb = texture(texSampler, ToSrgb(Tri(pos)*Mask(fragTexCoord.xy))) * fragColor.rgb;
  //outColor.a = fragColor.a;
  //outColor.rgb = ToSrgb(outColor.rgb);
  
  //vec2 q = fragTexCoord.xy / resolution.xy;
  //  vec2 uv = q;
  //  uv = curve( uv );
  //  vec3 oricol = texture( texSampler, vec2(q.x,q.y) ).xyz;
  //  vec3 col;
	//float x =  sin(0.3*1+uv.y*21.0)*sin(0.7*1+uv.y*29.0)*sin(0.3+0.33*1+uv.y*31.0)*0.0017;
  //
  //  col.r = texture(texSampler,vec2(x+uv.x+0.001,uv.y+0.001)).x+0.05;
  //  col.g = texture(texSampler,vec2(x+uv.x+0.000,uv.y-0.002)).y+0.05;
  //  col.b = texture(texSampler,vec2(x+uv.x-0.002,uv.y+0.000)).z+0.05;
  //  col.r += 0.08*texture(texSampler,0.75*vec2(x+0.025, -0.027)+vec2(uv.x+0.001,uv.y+0.001)).x;
  //  col.g += 0.05*texture(texSampler,0.75*vec2(x+-0.022, -0.02)+vec2(uv.x+0.000,uv.y-0.002)).y;
  //  col.b += 0.08*texture(texSampler,0.75*vec2(x+-0.02, -0.018)+vec2(uv.x-0.002,uv.y+0.000)).z;
  //
  //  col = clamp(col*0.6+0.4*col*col*1.0,0.0,1.0);
  //
  //  float vig = (0.0 + 1.0*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y));
	//col *= vec3(pow(vig,0.3));
  //
  //  col *= vec3(0.95,1.05,0.95);
	//col *= 2.8;
  //
	//float scans = clamp( 0.35+0.35*sin(3.5*1+uv.y*resolution.y*1.5), 0.0, 1.0);
	//
	//float s = pow(scans,1.7);
	//col = col*vec3( 0.4+0.7*s) ;
  //
  //  col *= 1.0+0.01*sin(110.0*1);
	//if (uv.x < 0.0 || uv.x > 1.0)
	//	col *= 0.0;
	//if (uv.y < 0.0 || uv.y > 1.0)
	//	col *= 0.0;
	//
	//col*=1.0-0.65*vec3(clamp((mod(fragTexCoord.x, 2.0)-1.0)*2.0,0.0,1.0));
	//
  //  float comp = smoothstep( 0.1, 0.9, sin(1) );
  //
	//// Remove the next line to stop cross-fade between original and postprocess
 ////	col = mix( col, oricol, comp );
  //
  //  outColor = vec4(col,1.0);
}