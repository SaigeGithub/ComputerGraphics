#version 330 core
out vec3 color;
in vec3 fpoint;
//in vec3 vpoint;
in float vheight; ///< for debug coloring
in float scale; ///< to normalize color range
in vec3 a;
in vec3 b;
in vec3 d;

in vec3 fnormal_cam;



uniform sampler2D grass;
uniform sampler2D water;
uniform sampler2D snow;
uniform sampler2D rock;
uniform sampler2D sand;

uniform sampler2D tex;

float tex_at(vec2 uv){ return texture(tex,uv).r; }
vec2 uv=1.0/300.0*vec2(fpoint.x+300, fpoint.z+300);
//1.0/300.0*vec2(fpoint.x+300, fpoint.z+300)


uniform mat4 MODEL,VIEW,PROJ;
//vec3 L =  (vec3 (VIEW*MODEL*vec4(0,1,0,0))).xyz;
//vec3 N = normalize(cross(dFdx(fpoint),dFdy(fpoint)));
//float diffuse = max(0.0f,dot(N,L));


vec3 NN = normalize(fnormal_cam);

vec3 LL =  (vec3 (VIEW*MODEL*vec4(0,1,0,0))).xyz; ///< now

float diffuse=dot(NN,LL)+0.2;

//vec3 B=fpoint+vec3(fpoint.x,scale * tex_at( 1.0/300.0*vec2(fpoint.x+300, fpoint.z+301) ),fpoint.z+1);
//vec3 C=fpoint+vec3(fpoint.x+1,scale * tex_at( 1.0/300.0*vec2(fpoint.x+301, fpoint.z+300) ),fpoint.z);

//float C_height = scale * tex_at( 1.0/300.0*vec2(C.x+300, C.z+300) );
//float B_height = scale * tex_at( 1.0/300.0*vec2(B.x+300, B.z+300) );

//B=vec3(B.x,B_height,B.z);
//C=vec3 (C.x(),C_height,C.z());
//vec3 AB= B-fpoint;
//vec3 AC= C-fpoint;
//vec3 L= vec3(0,1,0);
//vec3 N=normalize(cross(AB,AC));
//float diffuse=max(0.0f,dot(N,L));



//vec3 normal= vec3 cross(vec3(pointB-fpoint), vec3(pointC-fpoint));
//normal= normalize(normal);


//float f(float t)
//{
//    float t_3 = t * t * t;
//    return 6 * t * t * t_3 - 15 * t * t_3 + 10 * t_3;
//}

//float tex_at(vec2 uv){ return texture(tex,uv).r; }
mat4 MV = VIEW*MODEL;
float slope = abs(dot(NN,normalize((MV*vec4(0,1,0,0)).xyz)));
void main() {

    if(vheight< 2){
         color= diffuse* texture(water,uv).rgb;
    }
    else if(vheight>2 && vheight<10){
        color = diffuse*mix(texture(water,uv).rgb,texture(sand,uv).rgb, (vheight-2)/8);
    }else if(vheight>10 && vheight<30){


        color = diffuse*mix(texture(sand,uv).rgb,texture(grass,uv).rgb, (vheight-10)/20);
    }else if(vheight>30&& vheight<60){
        color=diffuse * texture(grass,uv).rgb;
    }else if(vheight>60 && vheight< 80){

        color = diffuse*mix(texture(grass,uv).rgb,texture(rock,uv).rgb, (vheight-60)/20);
        //color=texture(snow,uv).rgb;
    }
    else{
        if(slope<0.2){
        color= diffuse* texture(rock,uv).rgb;
        }else{
             color= diffuse*mix(texture(rock,uv).rgb,texture(snow,uv).rgb, (vheight-80)/20);}
        }
        //color = vec3(diffuse,diffuse, diffuse);
}
