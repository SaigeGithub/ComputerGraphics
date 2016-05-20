#version 330 core
out vec4 color;

in vec2 uv;
uniform sampler2D tex;
const vec4 COLORS[2] = vec4[](
    vec4(28/255,151/255,153/255,1.0),
    vec4(0.0,1.0,0.0,1.0));

void main() {
    //color = texture(tex,uv).rgba;
    //color = COLORS[gl_PrimitiveID];
    color = vec4(0.643137,0.1137254,0.1137254,1.0);
}


