#version 330 core
in vec3 vpoint;
out vec3 fpoint;
out float vheight;
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

//uniform sampler2D tex;
uniform sampler2D tex;

float tex_at(vec2 uv){ return texture(tex,uv).r; }

void main() {
       // fpoint = vpoint;
         //gl_Position = PROJ * VIEW * MODEL * vec4(vpoint, 1.0);
        //vheight = tex_at( 1.0/400.0*vec2(vpoint.x+300, vpoint.z+300) );
        vheight=1;

        gl_Position = PROJ * VIEW * MODEL * vec4(vpoint.x, vheight, vpoint.z, 1.0);
       fpoint=vec3 (vpoint.x,vheight,vpoint.z);
}
