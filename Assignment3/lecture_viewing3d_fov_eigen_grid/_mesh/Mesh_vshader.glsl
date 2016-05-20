#version 330 core
uniform float time;
in vec3 vpoint; ///< [-1,+1]
//in vec3 vnormal;
out float vheight; ///< for debug coloring
out float scale;
out vec3 fpoint;


out vec3 fnormal_cam;


uniform mat4 MODEL,VIEW,PROJ;



uniform sampler2D tex;

float tex_at(vec2 uv){ return texture(tex,uv).r; }

void main() {

    scale =400;
    vheight = scale * tex_at( 1.0/400.0*vec2(vpoint.x+300, vpoint.z+300) );

    vec3 normal=vec3(0,1,0);

    float a=(scale * tex_at( 1.0/400.0*vec2(vpoint.x+301, vpoint.z+300))) -(scale * tex_at( 1.0/400.0*vec2(vpoint.x+299, vpoint.z+300)));
    float b=(scale * tex_at( 1.0/400.0*vec2(vpoint.x+300, vpoint.z+301))) -(scale * tex_at( 1.0/400.0*vec2(vpoint.x+300, vpoint.z+299))) ;
    normal= normalize( vec3(0+a,1,0+b));


    gl_Position = PROJ * VIEW * MODEL * vec4(vpoint.x, vheight, vpoint.z, 1.0);

    //vec3 foint2 = (VIEW*MODEL*vec4(vpoint.x, vheight, vpoint.z, 1.0) ).xyz;
    fpoint=vec3 (vpoint.x,vheight,vpoint.z);
    fnormal_cam = inverse( transpose( mat3(VIEW * MODEL) )) * normal;
   // fnormal_cam=vec3(1,1,0);

}
