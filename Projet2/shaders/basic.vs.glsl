#version 120

attribute vec3 inPosition;
attribute vec3 inNormal;
attribute vec2 inTexCoord;

varying vec3 fragPos;
varying vec3 normal;
varying vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragPos = vec3(model * vec4(inPosition, 1.0));
    normal = mat3(model) * inNormal;
    texCoord = inTexCoord;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}
