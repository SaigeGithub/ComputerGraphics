#version 330 core
in vec4 fcolor; ///< passed by vshader
out vec4 color; ///< output color

void main() {
    color = fcolor; /// fcolor is interpolated!
}
