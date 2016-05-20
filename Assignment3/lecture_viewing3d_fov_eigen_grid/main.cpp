#include "icg_common.h"
#include "_mesh/Mesh.h"
#include "_skybox/skybox.h"
#include "_water/water.h"

#define     GLFW_KEY_W   87
#define     GLFW_KEY_A   65
#define     GLFW_KEY_S   83
#define     GLFW_KEY_D   68


int window_width = 1024;
int window_height = 640;

Mesh mesh;
skybox sky;
water waters;

float theta = 30; //< camera angle


//mousemove initial
float horizontalAngle =3.14f;
float verticalAngle=0.0f;
int xpos, ypos;
int oldX,oldY;

bool pressed_W=false;
bool pressed_A=false;
bool pressed_S=false;
bool pressed_D=false;

vec3 eye=vec3(1.0,1.0 ,1.0);
vec3 at=vec3(0.0,0.0 ,0.0);
vec3 up=vec3(0.0,1.0 ,0.0);


void init(){
    glfwEnable(GLFW_KEY_REPEAT);
    glClearColor(1,1,1, /*solid*/1.0 );
    glEnable(GL_DEPTH_TEST);
    mesh.init();
    sky.init();
    waters.init();
}


void keyboardWASD(int key, int action){
    if (action==GLFW_PRESS && key==GLFW_KEY_SPACE)
        mesh.wireframe = !mesh.wireframe;
        sky.wireframe =!sky.wireframe;
        waters.wireframe=!waters.wireframe;
    if(key == GLFW_KEY_W && action == GLFW_PRESS){
        pressed_W=true;
    }else pressed_W=false;
    if(key == GLFW_KEY_A && action == GLFW_PRESS){
        pressed_A=true;
    }else pressed_A=false;
    if(key == GLFW_KEY_S && action == GLFW_PRESS){
        pressed_S=true;
    }else pressed_S=false;
    if(key == GLFW_KEY_D && action == GLFW_PRESS){
        pressed_D=true;
    }else pressed_D=false;
}



void display(){
    opengp::update_title_fps("Saige's world");
    glViewport(0,0,window_width,window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ///--- Upload viewing matrices externally
    GLuint pid = mesh.getProgramID();
    glUseProgram(pid);
        mat4 MODEL = mat4::Identity();
        glUniformMatrix4fv(glGetUniformLocation(pid, "MODEL"), 1, GL_FALSE, MODEL.data());


//        int x = 0, y = 0;
//        glfwGetMousePos(&x, &y);
//        x=x/100;
//        y=y/100;
        if(pressed_W==true){
            eye= vec3(eye+(at-eye)/50);
            at= vec3(at+(at-eye)/50);
            //at=at+(at-eye);
//            eye.y()+=0.1*cos(M_PI/180*(horTheta-90));
//            eye.x()-=0.1*sin(M_PI/180*(horTheta-90));
        }else if(pressed_S==true){
            eye=vec3(eye-(at-eye)/50);
            at=vec3(at-(at-eye)/50);
            //at=at-(at-eye).normalized();
//            eye.y()-=0.1*cos(M_PI/180*(horTheta-90));
//            eye.x()+=0.1*sin(M_PI/180*(horTheta-90));
        }else if(pressed_A==true){
            vec3 left_vec=vec3((at-eye).cross(up));
            eye=vec3 ((eye-left_vec/50));
            at=vec3 ((at-left_vec/50));
            //eye.x()-=0.1*cos(M_PI/180*(horTheta-90));
            //eye.y()-=0.1*sin(M_PI/180*(horTheta-90));
//            vec3 left_vec=(up.cross(at));
//            left_vec=left_vec.normalized();
//            theta = std::min( theta+1, +89.f );
//            float theta_rad = M_PI/180.0*theta;
//            eye=vec3(eye.x()-0.1, eye.y(), eye.z());
//            at=vec3(at.x()-0.1, at.y(),at.z());
        }else if(pressed_D==true){
           vec3 left_vec=vec3((at-eye).cross(up));
            eye=vec3 ((eye+left_vec/50));
            at=vec3 ((at+left_vec/50));
//            vec3 right=crossProduct(at,up);
//            theta = std::min( theta+1, +89.f );
//            float theta_rad = M_PI/180.0*theta;
//            eye=vec3(eye.x()+0.1, eye.y(), eye.z());
//            at=vec3 (at.x()+0.1, at.y(), at.z());
            //eye=vec3(eye+right/10);
           // eye.x()+=0.1*cos(M_PI/180*(horTheta-90));
            //eye.y()+=0.1*sin(M_PI/180*(horTheta-90));
        }




        glfwGetMousePos(&xpos,&ypos);
        horizontalAngle += float(oldX - xpos )/100;
        verticalAngle   += float( oldY - ypos )/100;
        if (verticalAngle > (85 * M_PI / 180.0)){
            verticalAngle = 85 * M_PI / 180.0;
           // glfwSetMousePos(oldX, oldY);
        }
        if (verticalAngle < (-85 * M_PI / 180.0)){
            verticalAngle = -85 * M_PI / 180.0;
            //glfwSetMousePos(oldX, oldY);
                    }
        oldX=xpos;
        oldY=ypos;
        at=eye+ 10* (vec3(cos(verticalAngle)* sin(horizontalAngle),sin(verticalAngle),(cos(verticalAngle) * cos(horizontalAngle))).normalized());



        mat4 VIEW = Eigen::lookAt( 100*eye, 100*at, 100*up );
        glUniformMatrix4fv(glGetUniformLocation(pid, "VIEW"), 1, GL_FALSE, VIEW.data());


        //vec3 camera_pos( 2*cos(theta_rad)*100, 1  *100  ,2*100* sin(theta_rad));
        //mat4 VIEW = Eigen::lookAt( camera_pos, vec3(0,0,0), vec3(0,1,0) );
        glUniformMatrix4fv(glGetUniformLocation(pid, "VIEW"), 1, GL_FALSE, VIEW.data());
        
        mat4 PROJ = Eigen::perspective(75.0f, window_width/(float)window_height, 0.1f, 1000.0f);
        glUniformMatrix4fv(glGetUniformLocation(pid, "PROJ"), 1, GL_FALSE, PROJ.data());
    glUseProgram(pid);
    
    mesh.draw();

    pid=waters.getProgramID();
    glUseProgram(pid);
    glUniformMatrix4fv(glGetUniformLocation(pid, "MODEL"), 1, GL_FALSE, MODEL.data());
    glUniformMatrix4fv(glGetUniformLocation(pid, "VIEW"), 1, GL_FALSE, VIEW.data());
    glUniformMatrix4fv(glGetUniformLocation(pid, "PROJ"), 1, GL_FALSE, PROJ.data());
    waters.draw();


    pid=sky.getProgramID();
    glUseProgram(pid);
    glUniformMatrix4fv(glGetUniformLocation(pid, "MODEL"), 1, GL_FALSE, MODEL.data());
    glUniformMatrix4fv(glGetUniformLocation(pid, "VIEW"), 1, GL_FALSE, VIEW.data());
    glUniformMatrix4fv(glGetUniformLocation(pid, "PROJ"), 1, GL_FALSE, PROJ.data());
    sky.draw();


}




int main(int, char**){
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow();
    glfwSetKeyCallback(keyboardWASD);

    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}
