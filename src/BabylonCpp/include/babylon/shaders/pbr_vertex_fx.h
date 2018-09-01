﻿#ifndef BABYLON_SHADERS_PBR_VERTEX_FX_H
#define BABYLON_SHADERS_PBR_VERTEX_FX_H

namespace BABYLON {

extern const char* pbrVertexShader;

const char* pbrVertexShader
  = "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "#include<__decl__pbrVertex>\n"
    "\n"
    "// Attributes\n"
    "attribute vec3 position;\n"
    "#ifdef NORMAL\n"
    "attribute vec3 normal;\n"
    "#endif\n"
    "#ifdef TANGENT\n"
    "attribute vec4 tangent;\n"
    "#endif\n"
    "#ifdef UV1\n"
    "attribute vec2 uv;\n"
    "#endif\n"
    "#ifdef UV2\n"
    "attribute vec2 uv2;\n"
    "#endif\n"
    "#ifdef MAINUV1\n"
    "varying vec2 vMainUV1;\n"
    "#endif\n"
    "#ifdef MAINUV2\n"
    "varying vec2 vMainUV2; \n"
    "#endif \n"
    "#ifdef VERTEXCOLOR\n"
    "attribute vec4 color;\n"
    "#endif\n"
    "\n"
    "#include<helperFunctions>\n"
    "#include<bonesDeclaration>\n"
    "\n"
    "// Uniforms\n"
    "#include<instancesDeclaration>\n"
    "\n"
    "#if defined(ALBEDO) && ALBEDODIRECTUV == 0\n"
    "varying vec2 vAlbedoUV;\n"
    "#endif\n"
    "\n"
    "#if defined(AMBIENT) && AMBIENTDIRECTUV == 0\n"
    "varying vec2 vAmbientUV;\n"
    "#endif\n"
    "\n"
    "#if defined(OPACITY) && OPACITYDIRECTUV == 0\n"
    "varying vec2 vOpacityUV;\n"
    "#endif\n"
    "\n"
    "#if defined(EMISSIVE) && EMISSIVEDIRECTUV == 0\n"
    "varying vec2 vEmissiveUV;\n"
    "#endif\n"
    "\n"
    "#if defined(LIGHTMAP) && LIGHTMAPDIRECTUV == 0\n"
    "varying vec2 vLightmapUV;\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIVITY) && REFLECTIVITYDIRECTUV == 0\n"
    "varying vec2 vReflectivityUV;\n"
    "#endif\n"
    "\n"
    "#if defined(MICROSURFACEMAP) && MICROSURFACEMAPDIRECTUV == 0\n"
    "varying vec2 vMicroSurfaceSamplerUV;\n"
    "#endif\n"
    "\n"
    "#if defined(BUMP) && BUMPDIRECTUV == 0\n"
    "varying vec2 vBumpUV;\n"
    "#endif\n"
    "\n"
    "// Output\n"
    "varying vec3 vPositionW;\n"
    "#ifdef NORMAL\n"
    "  varying vec3 vNormalW;\n"
    "  #if defined(USESPHERICALFROMREFLECTIONMAP) && defined(USESPHERICALINVERTEX)\n"
    "  varying vec3 vEnvironmentIrradiance;\n"
    "  \n"
    "  #include<harmonicsFunctions>\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "varying vec4 vColor;\n"
    "#endif\n"
    "\n"
    "#include<bumpVertexDeclaration>\n"
    "#include<clipPlaneVertexDeclaration>\n"
    "#include<fogVertexDeclaration>\n"
    "#include<__decl__lightFragment>[0..maxSimultaneousLights]\n"
    "\n"
    "#include<morphTargetsVertexGlobalDeclaration>\n"
    "#include<morphTargetsVertexDeclaration>[0..maxSimultaneousMorphTargets]\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "varying vec3 vPositionUVW;\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "varying vec3 vDirectionW;\n"
    "#endif\n"
    "\n"
    "#include<logDepthDeclaration>\n"
    "\n"
    "void main(void) {\n"
    "  vec3 positionUpdated = position;\n"
    "#ifdef NORMAL\n"
    "  vec3 normalUpdated = normal;\n"
    "#endif\n"
    "#ifdef TANGENT\n"
    "  vec4 tangentUpdated = tangent;\n"
    "#endif\n"
    "\n"
    "#include<morphTargetsVertex>[0..maxSimultaneousMorphTargets]\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "  #ifdef REFLECTIONMAP_SKYBOX_TRANSFORMED\n"
    "  vPositionUVW = (reflectionMatrix * vec4(positionUpdated, 1.0)).xyz;\n"
    "  #else\n"
    "  vPositionUVW = positionUpdated;\n"
    "  #endif\n"
    "#endif \n"
    "\n"
    "#include<instancesVertex>\n"
    "#include<bonesVertex>\n"
    "\n"
    "  gl_Position = viewProjection * finalWorld * vec4(positionUpdated, 1.0);\n"
    "\n"
    "  vec4 worldPos = finalWorld * vec4(positionUpdated, 1.0);\n"
    "  vPositionW = vec3(worldPos);\n"
    "\n"
    "#ifdef NORMAL\n"
    "  mat3 normalWorld = mat3(finalWorld);\n"
    "\n"
    "  #ifdef NONUNIFORMSCALING\n"
    "  normalWorld = transposeMat3(inverseMat3(normalWorld));\n"
    "  #endif\n"
    "\n"
    "  vNormalW = normalize(normalWorld * normalUpdated);\n"
    "\n"
    "  #if defined(USESPHERICALFROMREFLECTIONMAP) && defined(USESPHERICALINVERTEX)\n"
    "  vec3 reflectionVector = vec3(reflectionMatrix * vec4(vNormalW, 0)).xyz;\n"
    "  #ifdef REFLECTIONMAP_OPPOSITEZ\n"
    "  reflectionVector.z *= -1.0;\n"
    "  #endif\n"
    "  vEnvironmentIrradiance = environmentIrradianceJones(reflectionVector);\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "  vDirectionW = normalize(vec3(finalWorld * vec4(positionUpdated, 0.0)));\n"
    "#endif\n"
    "\n"
    "  // Texture coordinates\n"
    "#ifndef UV1\n"
    "  vec2 uv = vec2(0., 0.);\n"
    "#endif\n"
    "#ifndef UV2\n"
    "  vec2 uv2 = vec2(0., 0.);\n"
    "#endif\n"
    "\n"
    "#ifdef MAINUV1\n"
    "  vMainUV1 = uv;\n"
    "#endif \n"
    "\n"
    "#ifdef MAINUV2\n"
    "  vMainUV2 = uv2;\n"
    "#endif \n"
    "\n"
    "#if defined(ALBEDO) && ALBEDODIRECTUV == 0 \n"
    "  if (vAlbedoInfos.x == 0.)\n"
    "  {\n"
    "  vAlbedoUV = vec2(albedoMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vAlbedoUV = vec2(albedoMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(AMBIENT) && AMBIENTDIRECTUV == 0 \n"
    "  if (vAmbientInfos.x == 0.)\n"
    "  {\n"
    "  vAmbientUV = vec2(ambientMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vAmbientUV = vec2(ambientMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(OPACITY) && OPACITYDIRECTUV == 0 \n"
    "  if (vOpacityInfos.x == 0.)\n"
    "  {\n"
    "  vOpacityUV = vec2(opacityMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vOpacityUV = vec2(opacityMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(EMISSIVE) && EMISSIVEDIRECTUV == 0 \n"
    "  if (vEmissiveInfos.x == 0.)\n"
    "  {\n"
    "  vEmissiveUV = vec2(emissiveMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vEmissiveUV = vec2(emissiveMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(LIGHTMAP) && LIGHTMAPDIRECTUV == 0 \n"
    "  if (vLightmapInfos.x == 0.)\n"
    "  {\n"
    "  vLightmapUV = vec2(lightmapMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vLightmapUV = vec2(lightmapMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIVITY) && REFLECTIVITYDIRECTUV == 0 \n"
    "  if (vReflectivityInfos.x == 0.)\n"
    "  {\n"
    "  vReflectivityUV = vec2(reflectivityMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vReflectivityUV = vec2(reflectivityMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(MICROSURFACEMAP) && MICROSURFACEMAPDIRECTUV == 0 \n"
    "  if (vMicroSurfaceSamplerInfos.x == 0.)\n"
    "  {\n"
    "  vMicroSurfaceSamplerUV = vec2(microSurfaceSamplerMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vMicroSurfaceSamplerUV = vec2(microSurfaceSamplerMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(BUMP) && BUMPDIRECTUV == 0 \n"
    "  if (vBumpInfos.x == 0.)\n"
    "  {\n"
    "  vBumpUV = vec2(bumpMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vBumpUV = vec2(bumpMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "  // TBN\n"
    "#include<bumpVertex>\n"
    "\n"
    "  // Clip plane\n"
    "#include<clipPlaneVertex>\n"
    "\n"
    "  // Fog\n"
    "#include<fogVertex>\n"
    "\n"
    "  // Shadows\n"
    "#include<shadowsVertex>[0..maxSimultaneousLights]\n"
    "\n"
    "  // Vertex color\n"
    "#ifdef VERTEXCOLOR\n"
    "  vColor = color;\n"
    "#endif\n"
    "\n"
    "  // Point size\n"
    "#ifdef POINTSIZE\n"
    "  gl_PointSize = pointSize;\n"
    "#endif\n"
    "\n"
    "  // Log. depth\n"
    "#include<logDepthVertex>\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_PBR_VERTEX_FX_H
