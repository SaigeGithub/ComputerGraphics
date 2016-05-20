#version 330 core
out vec3 color;
in vec3 fpoint;

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform samplerCube skybox_tex;

vec3 Red=vec3(1,0,1);

void main(){
   // color = Red;
    color=texture(skybox_tex,fpoint.xyz).rgb;
}
