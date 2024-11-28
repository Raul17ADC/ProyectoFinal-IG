#version 330 core

layout(location = 0)in vec3 inPosition;
layout(location = 1)in vec3 inNormal;

uniform mat4 uN;
uniform mat4 uM;
uniform mat4 uPVM;

out vec3 vNor;
out vec3 vPos;

void main() {
    vec4 inPositionH = vec4(inPosition, 1);
    vec4 inNormalH = vec4(inNormal, 0);
    
    gl_Position = uPVM * inPositionH;
    
    vNor = (uN * inNormalH).xyz;
    vPos = (uM * inPositionH).xyz;
}
