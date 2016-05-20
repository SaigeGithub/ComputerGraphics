#version 330 core
in vec3 vpoint;
out vec3 fpoint;
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

//uniform sampler2D tex;

void main() {
    fpoint = vpoint;
    gl_Position = PROJ * VIEW * MODEL * vec4(vpoint, 1.0);
}
