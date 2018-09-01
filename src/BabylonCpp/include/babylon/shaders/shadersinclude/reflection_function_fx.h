﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_REFLECTION_FUNCTION_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_REFLECTION_FUNCTION_FX_H

namespace BABYLON {

extern const char* reflectionFunction;

const char* reflectionFunction
  = "#ifdef USE_LOCAL_REFLECTIONMAP_CUBIC\n"
    "vec3 parallaxCorrectNormal( vec3 vertexPos, vec3 origVec, vec3 cubeSize, vec3 cubePos ) {\n"
    "  // Find the ray intersection with box plane\n"
    "  vec3 invOrigVec = vec3(1.0,1.0,1.0) / origVec;\n"
    "  vec3 halfSize = cubeSize * 0.5;\n"
    "  vec3 intersecAtMaxPlane = (cubePos + halfSize - vertexPos) * invOrigVec;\n"
    "  vec3 intersecAtMinPlane = (cubePos - halfSize - vertexPos) * invOrigVec;\n"
    "  // Get the largest intersection values (we are not intersted in negative values)\n"
    "  vec3 largestIntersec = max(intersecAtMaxPlane, intersecAtMinPlane);\n"
    "  // Get the closest of all solutions\n"
    "  float distance = min(min(largestIntersec.x, largestIntersec.y), largestIntersec.z);\n"
    "  // Get the intersection position\n"
    "  vec3 intersectPositionWS = vertexPos + origVec * distance;\n"
    "  // Get corrected vector\n"
    "  return intersectPositionWS - cubePos;\n"
    "}\n"
    "#endif\n"
    "\n"
    "vec3 computeReflectionCoords(vec4 worldPos, vec3 worldNormal)\n"
    "{\n"
    "#if defined(REFLECTIONMAP_EQUIRECTANGULAR_FIXED) || defined(REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED)\n"
    "  vec3 direction = normalize(vDirectionW);\n"
    "  float lon = atan(direction.z, direction.x);\n"
    "  float lat = acos(direction.y);\n"
    "  vec2 sphereCoords = vec2(lon, lat) * RECIPROCAL_PI2 * 2.0;\n"
    "  float s = sphereCoords.x * 0.5 + 0.5;\n"
    "  float t = sphereCoords.y;  \n"
    "\n"
    "   #ifdef REFLECTIONMAP_MIRROREDEQUIRECTANGULAR_FIXED\n"
    "  return vec3(1.0 - s, t, 0);\n"
    "  #else\n"
    "  return vec3(s, t, 0);\n"
    "  #endif\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_EQUIRECTANGULAR\n"
    "\n"
    "  vec3 cameraToVertex = normalize(worldPos.xyz - vEyePosition.xyz);\n"
    "  vec3 r = normalize(reflect(cameraToVertex, worldNormal));\n"
    "  float lon = atan(r.z, r.x);\n"
    "  float lat = acos(r.y);\n"
    "  vec2 sphereCoords = vec2(lon, lat) * RECIPROCAL_PI2 * 2.0;\n"
    "  float s = sphereCoords.x * 0.5 + 0.5;\n"
    "  float t = sphereCoords.y;    \n"
    "\n"
    "  return vec3(s, t, 0);\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_SPHERICAL\n"
    "  vec3 viewDir = normalize(vec3(view * worldPos));\n"
    "  vec3 viewNormal = normalize(vec3(view * vec4(worldNormal, 0.0)));\n"
    "\n"
    "  vec3 r = reflect(viewDir, viewNormal);\n"
    "  r.z = r.z - 1.0;\n"
    "\n"
    "  float m = 2.0 * length(r);\n"
    "\n"
    "  return vec3(r.x / m + 0.5, 1.0 - r.y / m - 0.5, 0);\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_PLANAR\n"
    "  vec3 viewDir = worldPos.xyz - vEyePosition.xyz;\n"
    "  vec3 coords = normalize(reflect(viewDir, worldNormal));\n"
    "\n"
    "  return vec3(reflectionMatrix * vec4(coords, 1));\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_CUBIC\n"
    "  vec3 viewDir = normalize(worldPos.xyz - vEyePosition.xyz);\n"
    "\n"
    "  // worldNormal has already been normalized.\n"
    "  vec3 coords = reflect(viewDir, worldNormal);\n"
    "\n"
    "  #ifdef USE_LOCAL_REFLECTIONMAP_CUBIC\n"
    "  coords = parallaxCorrectNormal(worldPos.xyz, coords, vReflectionSize, vReflectionPosition);\n"
    "  #endif\n"
    "\n"
    "  coords = vec3(reflectionMatrix * vec4(coords, 0));\n"
    "  \n"
    "  #ifdef INVERTCUBICMAP\n"
    "  coords.y *= -1.0;\n"
    "  #endif\n"
    "\n"
    "  return coords;\n"
    "#endif\n"
    "\n"
    "\n"
    "#ifdef REFLECTIONMAP_PROJECTION\n"
    "  return vec3(reflectionMatrix * (view * worldPos));\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "  return vPositionUVW;\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_EXPLICIT\n"
    "  return vec3(0, 0, 0);\n"
    "#endif\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_REFLECTION_FUNCTION_FX_H
