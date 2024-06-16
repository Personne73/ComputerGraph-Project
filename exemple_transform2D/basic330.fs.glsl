#version 330

uniform float u_Time;

// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

in vec3 v_Color;

// la variable en sortie du fragment shader peut 
// maintenant porter n'importe quel nom avec 'out'
out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(v_Color, 1.0);
}