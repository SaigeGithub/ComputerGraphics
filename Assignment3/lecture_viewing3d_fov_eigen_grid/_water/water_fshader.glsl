#version 330 core
out vec3 color;
in vec3 fpoint;
in float vheight;
//in vec3 fpoint;

uniform mat4 MODEL;
uniform mat4 VIEW;
//uniform samplerCube skybox_tex;

uniform sampler2D water;
uniform float time;

vec3 Red=vec3(1,0,1);
vec2 uv=(1.0/300.0)*vec2(fpoint.x, fpoint.z)+time;
//get the normal from the texture
vec3 N=normalize(2*texture(water,uv).rbg-vec3(1,1,1));

vec3 L =  normalize(vec3 (1,1,1)); ///< now

float diffuse=dot(N,L);

void main(){

    color= diffuse*vec3 (0,0.5,1);
}

