// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

#version 120

varying vec3 v_Normal;
varying vec2 v_TexCoord;

void main()
{
    gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0); // couleur orange
}