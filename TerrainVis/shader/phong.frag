#version 430

in vec3 LightIntensity;
in vec3 Color;

layout( location = 0 ) out vec4 FragColor;

void main() {
    vec3 c = Color + LightIntensity;
    FragColor = vec4(c,1.0f);
}
