﻿#ifndef BABYLON_SHADERS_BACKGROUND_FRAGMENT_FX_H
#define BABYLON_SHADERS_BACKGROUND_FRAGMENT_FX_H

namespace BABYLON {

extern const char* backgroundPixelShader;

const char* backgroundPixelShader
  = "#ifdef TEXTURELODSUPPORT\n"
    "#extension GL_EXT_shader_texture_lod : enable\n"
    "#endif\n"
    "\n"
    "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "#include<__decl__backgroundFragment>\n"
    "\n"
    "#define RECIPROCAL_PI2 0.15915494\n"
    "\n"
    "// Constants\n"
    "uniform vec3 vEyePosition;\n"
    "\n"
    "// Input\n"
    "varying vec3 vPositionW;\n"
    "\n"
    "#ifdef MAINUV1\n"
    "  varying vec2 vMainUV1;\n"
    "#endif \n"
    "\n"
    "#ifdef MAINUV2 \n"
    "  varying vec2 vMainUV2; \n"
    "#endif \n"
    "\n"
    "#ifdef NORMAL\n"
    "varying vec3 vNormalW;\n"
    "#endif\n"
    "\n"
    "#ifdef DIFFUSE\n"
    "  #if DIFFUSEDIRECTUV == 1\n"
    "  #define vDiffuseUV vMainUV1\n"
    "  #elif DIFFUSEDIRECTUV == 2\n"
    "  #define vDiffuseUV vMainUV2\n"
    "  #else\n"
    "  varying vec2 vDiffuseUV;\n"
    "  #endif\n"
    "  uniform sampler2D diffuseSampler;\n"
    "#endif\n"
    "\n"
    "// Reflection\n"
    "#ifdef REFLECTION\n"
    "  #ifdef REFLECTIONMAP_3D\n"
    "  #define sampleReflection(s, c) textureCube(s, c)\n"
    "\n"
    "  uniform samplerCube reflectionSampler;\n"
    "  \n"
    "  #ifdef TEXTURELODSUPPORT\n"
    "  #define sampleReflectionLod(s, c, l) textureCubeLodEXT(s, c, l)\n"
    "  #else\n"
    "  uniform samplerCube reflectionSamplerLow;\n"
    "  uniform samplerCube reflectionSamplerHigh;\n"
    "  #endif\n"
    "  #else\n"
    "  #define sampleReflection(s, c) texture2D(s, c)\n"
    "\n"
    "  uniform sampler2D reflectionSampler;\n"
    "\n"
    "  #ifdef TEXTURELODSUPPORT\n"
    "  #define sampleReflectionLod(s, c, l) texture2DLodEXT(s, c, l)\n"
    "  #else\n"
    "  uniform samplerCube reflectionSamplerLow;\n"
    "  uniform samplerCube reflectionSamplerHigh;\n"
    "  #endif\n"
    "  #endif\n"
    "\n"
    "  #ifdef REFLECTIONMAP_SKYBOX\n"
    "  varying vec3 vPositionUVW;\n"
    "  #else\n"
    "  #if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "  varying vec3 vDirectionW;\n"
    "  #endif\n"
    "  #endif\n"
    "\n"
    "  #include<reflectionFunction>\n"
    "#endif\n"
    "\n"
    "// Forces linear space for image processing\n"
    "#ifndef FROMLINEARSPACE\n"
    "  #define FROMLINEARSPACE;\n"
    "#endif\n"
    "\n"
    "// Prevent expensive light computations\n"
    "#ifndef SHADOWONLY\n"
    "  #define SHADOWONLY;\n"
    "#endif\n"
    "\n"
    "\n"
    "#include<imageProcessingDeclaration>\n"
    "\n"
    "// Lights\n"
    "#include<__decl__lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "#include<helperFunctions>\n"
    "#include<lightsFragmentFunctions>\n"
    "#include<shadowsFragmentFunctions>\n"
    "#include<imageProcessingFunctions>\n"
    "\n"
    "#include<clipPlaneFragmentDeclaration>\n"
    "\n"
    "// Fog\n"
    "#include<fogFragmentDeclaration>\n"
    "\n"
    "#ifdef REFLECTIONFRESNEL\n"
    "  #define FRESNEL_MAXIMUM_ON_ROUGH 0.25\n"
    "\n"
    "  vec3 fresnelSchlickEnvironmentGGX(float VdotN, vec3 reflectance0, vec3 reflectance90, float smoothness)\n"
    "  {\n"
    "  // Schlick fresnel approximation, extended with basic smoothness term so that rough surfaces do not approach reflectance90 at grazing angle\n"
    "  float weight = mix(FRESNEL_MAXIMUM_ON_ROUGH, 1.0, smoothness);\n"
    "  return reflectance0 + weight * (reflectance90 - reflectance0) * pow(clamp(1.0 - VdotN, 0., 1.), 5.0);\n"
    "  }\n"
    "#endif\n"
    "\n"
    "void main(void) {\n"
    "#include<clipPlaneFragment>\n"
    "\n"
    "  vec3 viewDirectionW = normalize(vEyePosition - vPositionW);\n"
    "\n"
    "// _____________________________ Normal Information ______________________________\n"
    "#ifdef NORMAL\n"
    "  vec3 normalW = normalize(vNormalW);\n"
    "#else\n"
    "  vec3 normalW = vec3(0.0, 1.0, 0.0);\n"
    "#endif\n"
    "\n"
    "// _____________________________ Light Information _______________________________\n"
    "  float shadow = 1.;\n"
    "  float globalShadow = 0.;\n"
    "  float shadowLightCount = 0.;\n"
    "\n"
    "#include<lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "#ifdef SHADOWINUSE\n"
    "  globalShadow /= shadowLightCount;\n"
    "#else\n"
    "  globalShadow = 1.0;\n"
    "#endif\n"
    "\n"
    "// _____________________________ REFLECTION ______________________________________\n"
    "vec4 reflectionColor = vec4(1., 1., 1., 1.);\n"
    "#ifdef REFLECTION\n"
    "  vec3 reflectionVector = computeReflectionCoords(vec4(vPositionW, 1.0), normalW);\n"
    "  #ifdef REFLECTIONMAP_OPPOSITEZ\n"
    "  reflectionVector.z *= -1.0;\n"
    "  #endif\n"
    "\n"
    "  // _____________________________ 2D vs 3D Maps ________________________________\n"
    "  #ifdef REFLECTIONMAP_3D\n"
    "  vec3 reflectionCoords = reflectionVector;\n"
    "  #else\n"
    "  vec2 reflectionCoords = reflectionVector.xy;\n"
    "  #ifdef REFLECTIONMAP_PROJECTION\n"
    "  reflectionCoords /= reflectionVector.z;\n"
    "  #endif\n"
    "  reflectionCoords.y = 1.0 - reflectionCoords.y;\n"
    "  #endif\n"
    "\n"
    "  #ifdef REFLECTIONBLUR\n"
    "  float reflectionLOD = vReflectionInfos.y;\n"
    "\n"
    "  #ifdef TEXTURELODSUPPORT\n"
    "  // Apply environment convolution scale/offset filter tuning parameters to the mipmap LOD selection\n"
    "  reflectionLOD = reflectionLOD * log2(vReflectionMicrosurfaceInfos.x) * vReflectionMicrosurfaceInfos.y + vReflectionMicrosurfaceInfos.z;\n"
    "  reflectionColor = sampleReflectionLod(reflectionSampler, reflectionCoords, reflectionLOD);\n"
    "  #else\n"
    "  float lodReflectionNormalized = clamp(reflectionLOD, 0., 1.);\n"
    "  float lodReflectionNormalizedDoubled = lodReflectionNormalized * 2.0;\n"
    "\n"
    "  vec4 reflectionSpecularMid = sampleReflection(reflectionSampler, reflectionCoords);\n"
    "  if(lodReflectionNormalizedDoubled < 1.0){\n"
    "  reflectionColor = mix(\n"
    "  sampleReflection(reflectionSamplerHigh, reflectionCoords),\n"
    "  reflectionSpecularMid,\n"
    "  lodReflectionNormalizedDoubled\n"
    "  );\n"
    "  } else {\n"
    "  reflectionColor = mix(\n"
    "  reflectionSpecularMid,\n"
    "  sampleReflection(reflectionSamplerLow, reflectionCoords),\n"
    "  lodReflectionNormalizedDoubled - 1.0\n"
    "  );\n"
    "  }\n"
    "  #endif\n"
    "  #else\n"
    "  vec4 reflectionSample = sampleReflection(reflectionSampler, reflectionCoords);\n"
    "  reflectionColor = reflectionSample;\n"
    "  #endif\n"
    "\n"
    "  #ifdef RGBDREFLECTION\n"
    "  reflectionColor.rgb = fromRGBD(reflectionColor);\n"
    "  #endif\n"
    "\n"
    "  #ifdef GAMMAREFLECTION\n"
    "  reflectionColor.rgb = toLinearSpace(reflectionColor.rgb);\n"
    "  #endif\n"
    "\n"
    "  #ifdef REFLECTIONBGR\n"
    "  reflectionColor.rgb = reflectionColor.bgr;\n"
    "  #endif\n"
    "\n"
    "  // _____________________________ Levels _____________________________________\n"
    "  reflectionColor.rgb *= vReflectionInfos.x;\n"
    "#endif\n"
    "\n"
    "// _____________________________ Diffuse Information _______________________________\n"
    "vec3 diffuseColor = vec3(1., 1., 1.);\n"
    "float finalAlpha = alpha;\n"
    "#ifdef DIFFUSE\n"
    "  vec4 diffuseMap = texture2D(diffuseSampler, vDiffuseUV);\n"
    "  #ifdef GAMMADIFFUSE\n"
    "  diffuseMap.rgb = toLinearSpace(diffuseMap.rgb);\n"
    "  #endif\n"
    "\n"
    "// _____________________________ Levels _____________________________________\n"
    "  diffuseMap.rgb *= vDiffuseInfos.y;\n"
    "\n"
    "  #ifdef DIFFUSEHASALPHA\n"
    "  finalAlpha *= diffuseMap.a;\n"
    "  #endif\n"
    "\n"
    "  diffuseColor = diffuseMap.rgb;\n"
    "#endif\n"
    "\n"
    "// _____________________________ MIX ________________________________________\n"
    "#ifdef REFLECTIONFRESNEL\n"
    "  vec3 colorBase = diffuseColor;\n"
    "#else\n"
    "  vec3 colorBase = reflectionColor.rgb * diffuseColor;\n"
    "#endif\n"
    "  colorBase = max(colorBase, 0.0);\n"
    "\n"
    "// ___________________________ COMPOSE _______________________________________\n"
    "#ifdef USERGBCOLOR\n"
    "  vec3 finalColor = colorBase;\n"
    "#else\n"
    "  #ifdef USEHIGHLIGHTANDSHADOWCOLORS\n"
    "  vec3 mainColor = mix(vPrimaryColorShadow.rgb, vPrimaryColor.rgb, colorBase);\n"
    "  #else\n"
    "  vec3 mainColor = vPrimaryColor.rgb;\n"
    "  #endif\n"
    "\n"
    "  vec3 finalColor = colorBase * mainColor;\n"
    "#endif\n"
    "\n"
    "// ___________________________ FRESNELS _______________________________________\n"
    "#ifdef REFLECTIONFRESNEL\n"
    "  vec3 reflectionAmount = vReflectionControl.xxx;\n"
    "  vec3 reflectionReflectance0 = vReflectionControl.yyy;\n"
    "  vec3 reflectionReflectance90 = vReflectionControl.zzz;\n"
    "  float VdotN = dot(normalize(vEyePosition), normalW);\n"
    "\n"
    "  vec3 planarReflectionFresnel = fresnelSchlickEnvironmentGGX(clamp(VdotN, 0.0, 1.0), reflectionReflectance0, reflectionReflectance90, 1.0);\n"
    "  reflectionAmount *= planarReflectionFresnel;\n"
    "\n"
    "  #ifdef REFLECTIONFALLOFF\n"
    "  float reflectionDistanceFalloff = 1.0 - clamp(length(vPositionW.xyz - vBackgroundCenter) * vReflectionControl.w, 0.0, 1.0);\n"
    "  reflectionDistanceFalloff *= reflectionDistanceFalloff;\n"
    "  reflectionAmount *= reflectionDistanceFalloff;\n"
    "  #endif\n"
    "\n"
    "  finalColor = mix(finalColor, reflectionColor.rgb, clamp(reflectionAmount, 0., 1.));\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITYFRESNEL\n"
    "  float viewAngleToFloor = dot(normalW, normalize(vEyePosition - vBackgroundCenter));\n"
    "\n"
    "  // Fade out the floor plane as the angle between the floor and the camera tends to 0 (starting from startAngle)\n"
    "  const float startAngle = 0.1;\n"
    "  float fadeFactor = clamp(viewAngleToFloor/startAngle, 0.0, 1.0);\n"
    "\n"
    "  finalAlpha *= fadeFactor * fadeFactor;\n"
    "#endif\n"
    "\n"
    "// ___________________________ SHADOWS _______________________________________\n"
    "#ifdef SHADOWINUSE\n"
    "  finalColor = mix(finalColor * shadowLevel, finalColor, globalShadow);\n"
    "#endif\n"
    "\n"
    "// ___________________________ FINALIZE _______________________________________\n"
    "vec4 color = vec4(finalColor, finalAlpha);\n"
    "\n"
    "#include<fogFragment>\n"
    "\n"
    "#ifdef IMAGEPROCESSINGPOSTPROCESS\n"
    "  // Sanitize output incase invalid normals or tangents have caused div by 0 or undefined behavior\n"
    "  // this also limits the brightness which helpfully reduces over-sparkling in bloom (native handles this in the bloom blur shader)\n"
    "  color.rgb = clamp(color.rgb, 0., 30.0);\n"
    "#else\n"
    "  // Alway run even to ensure going back to gamma space.\n"
    "  color = applyImageProcessing(color);\n"
    "#endif\n"
    "\n"
    "#ifdef PREMULTIPLYALPHA\n"
    "  // Convert to associative (premultiplied) format if needed.\n"
    "  color.rgb *= color.a;\n"
    "#endif\n"
    "\n"
    "#ifdef NOISE\n"
    "  color.rgb += dither(vPositionW.xy, 0.5);\n"
    "  color = max(color, 0.0);\n"
    "#endif\n"
    "\n"
    "  gl_FragColor = color;\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_BACKGROUND_FRAGMENT_FX_H
