#version 330 core

in vec3 vPos;
out vec4 outColor;

void main() {
    outColor = vec4(abs(sin(15.0 * vPos.x) * sin(15.0 * vPos.y))) + vec4(0.1, 0.3, 0.1, 1.0);
}
