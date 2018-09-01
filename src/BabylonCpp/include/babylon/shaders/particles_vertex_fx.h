﻿#ifndef BABYLON_SHADERS_PARTICLES_VERTEX_FX_H
#define BABYLON_SHADERS_PARTICLES_VERTEX_FX_H

namespace BABYLON {

extern const char* particlesVertexShader;

const char* particlesVertexShader
  = "// Attributes\n"
    "attribute vec3 position;\n"
    "attribute vec4 color;\n"
    "attribute float angle;\n"
    "attribute vec2 size;\n"
    "#ifdef ANIMATESHEET  \n"
    "attribute float cellIndex;\n"
    "#endif\n"
    "#ifndef BILLBOARD  \n"
    "attribute vec3 direction;\n"
    "#endif\n"
    "attribute vec2 offset;\n"
    "\n"
    "// Uniforms\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform vec2 translationPivot;\n"
    "\n"
    "#ifdef ANIMATESHEET  \n"
    "uniform vec3 particlesInfos; // x (number of rows) y(number of columns) z(rowSize)\n"
    "#endif\n"
    "\n"
    "// Output\n"
    "varying vec2 vUV;\n"
    "varying vec4 vColor;\n"
    "\n"
    "#if defined(CLIPPLANE) || defined(CLIPPLANE2) || defined(CLIPPLANE3) || defined(CLIPPLANE4)\n"
    "uniform mat4 invView;\n"
    "#endif\n"
    "#include<clipPlaneVertexDeclaration>\n"
    "\n"
    "#ifdef BILLBOARD\n"
    "  uniform vec3 eyePosition;  \n"
    "#endif\n"
    "\n"
    "vec3 rotate(vec3 yaxis, vec3 rotatedCorner) {\n"
    "  vec3 xaxis = normalize(cross(vec3(0., 1.0, 0.), yaxis));\n"
    "  vec3 zaxis = normalize(cross(yaxis, xaxis));\n"
    "\n"
    "  vec3 row0 = vec3(xaxis.x, xaxis.y, xaxis.z);\n"
    "  vec3 row1 = vec3(yaxis.x, yaxis.y, yaxis.z);\n"
    "  vec3 row2 = vec3(zaxis.x, zaxis.y, zaxis.z);\n"
    "\n"
    "  mat3 rotMatrix =  mat3(row0, row1, row2);\n"
    "\n"
    "  vec3 alignedCorner = rotMatrix * rotatedCorner;\n"
    "  return position + alignedCorner; \n"
    "}\n"
    "\n"
    "void main(void) {  \n"
    "  vec2 cornerPos;\n"
    "  \n"
    "  cornerPos = (vec2(offset.x - 0.5, offset.y  - 0.5) - translationPivot) * size + translationPivot;\n"
    "\n"
    "#ifdef BILLBOARD  \n"
    "  // Rotate\n"
    "  vec3 rotatedCorner;\n"
    "\n"
    "#ifdef BILLBOARDY  \n"
    "  rotatedCorner.x = cornerPos.x * cos(angle) - cornerPos.y * sin(angle);\n"
    "  rotatedCorner.z = cornerPos.x * sin(angle) + cornerPos.y * cos(angle);\n"
    "  rotatedCorner.y = 0.;\n"
    "\n"
    "  vec3 yaxis = position - eyePosition;\n"
    "  yaxis.y = 0.;\n"
    "  \n"
    "  vec3 worldPos = rotate(normalize(yaxis), rotatedCorner);\n"
    "  \n"
    "  vec3 viewPos = (view * vec4(worldPos, 1.0)).xyz; \n"
    "#else\n"
    "  rotatedCorner.x = cornerPos.x * cos(angle) - cornerPos.y * sin(angle);\n"
    "  rotatedCorner.y = cornerPos.x * sin(angle) + cornerPos.y * cos(angle);\n"
    "  rotatedCorner.z = 0.;\n"
    "\n"
    "  vec3 viewPos = (view * vec4(position, 1.0)).xyz + rotatedCorner; \n"
    "#endif\n"
    "\n"
    "  // Position\n"
    "  gl_Position = projection * vec4(viewPos, 1.0);   \n"
    "#else\n"
    "  // Rotate\n"
    "  vec3 rotatedCorner;\n"
    "  rotatedCorner.x = cornerPos.x * cos(angle) - cornerPos.y * sin(angle);\n"
    "  rotatedCorner.z = cornerPos.x * sin(angle) + cornerPos.y * cos(angle);\n"
    "  rotatedCorner.y = 0.;\n"
    "\n"
    "  vec3 yaxis = normalize(direction);\n"
    "  vec3 worldPos = rotate(yaxis, rotatedCorner);\n"
    "\n"
    "  gl_Position = projection * view * vec4(worldPos, 1.0);  \n"
    "#endif  \n"
    "  vColor = color;\n"
    "\n"
    "  #ifdef ANIMATESHEET\n"
    "  float rowOffset = floor(cellIndex / particlesInfos.z);\n"
    "  float columnOffset = cellIndex - rowOffset * particlesInfos.z;\n"
    "\n"
    "  vec2 uvScale = particlesInfos.xy;\n"
    "  vec2 uvOffset = vec2(offset.x , 1.0 - offset.y);\n"
    "  vUV = (uvOffset + vec2(columnOffset, rowOffset)) * uvScale;\n"
    "  #else\n"
    "  vUV = offset;\n"
    "  #endif\n"
    "\n"
    "  // Clip plane\n"
    "#if defined(CLIPPLANE) || defined(CLIPPLANE2) || defined(CLIPPLANE3) || defined(CLIPPLANE4)\n"
    "  vec4 worldPos = invView * vec4(viewPos, 1.0);\n"
    "#endif\n"
    "  #include<clipPlaneVertex>\n"
    "\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_PARTICLES_VERTEX_FX_H
