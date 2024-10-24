#version 420

layout(std140, binding=1) uniform Transformations {
 vec2 scale;
 vec2 translation;
 mat2 rotation;
 };

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;

out vec4 vertexColor;

void main() {
    gl_Position.xy = rotation*(scale*a_vertex_position.xy)+translation;
    gl_Position.zw = a_vertex_position.zw;  
    vertexColor = a_vertex_color;
}