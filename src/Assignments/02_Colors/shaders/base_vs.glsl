#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;

out vec4 vertexColor;

void main() {
    vertexColor = a_vertex_color;
    gl_Position = a_vertex_position;
}