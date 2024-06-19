// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

#version 120

varying vec3 FragPos;
varying vec3 Normal;
varying vec2 TexCoord;

uniform sampler2D texture_diffuse;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 diffuse = diff * texture2D(texture_diffuse, TexCoord).rgb;
    gl_FragColor = vec4(diffuse, 1.0);
}
