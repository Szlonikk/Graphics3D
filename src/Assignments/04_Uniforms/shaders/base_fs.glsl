#version 420

layout(std140, binding = 0) uniform Mixer {
    float strength;
    vec3 mix_color;
};

in vec4 vertexColor;

out vec4 vFragColor;

void main() {
    vec3 mixed_color = mix(vertexColor.rgb, mix_color, strength);
    vFragColor = vec4(mixed_color, vertexColor.a);
}
