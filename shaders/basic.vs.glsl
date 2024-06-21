#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec3 vNormal;
varying vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vNormal = normal;
    vTexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
