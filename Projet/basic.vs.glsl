// un VS doit toujours ecrire dans
// gl_Position qui est un vec4

#version 120

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoord;

uniform float u_Time;

varying vec3 v_Normal;
varying vec2 v_TexCoord;

void main()
{
    vec3 pos = a_Position;
    v_Normal = a_Normal;
    v_TexCoord = a_TexCoord;
    pos.x += sin(u_Time);

    gl_Position = vec4(pos, 1.0);
}