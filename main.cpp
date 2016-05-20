#include "icg_common.h"
#include "Quad.h"
#include "FrameBuffer.h"
#include "_screenquad/ScreenQuad.h"


int width=720, height=720;
typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
//initial some value I will be used in biezerCurve
std::vector<vec3> FourPoint;
std::vector<vec3> BeizerCurve;
float t;
float increasing;

//initial the values be used in transformation
const int SPEED_FACTOR = 1;

//All the quad parts are doing the miku
Quad miku_head;
Quad hair;
Quad hair_right;
Quad hairBound1;
Quad hairBound2;
Quad background;
Quad foreground;
Quad body;
Quad hand1;
Quad hand2;
Quad foot1;
Quad foot2;
Quad onion;

//frameBuffer part is doing the "let it snow"
FrameBuffer fb(width, height);
ScreenQuad squad;


//me readfile read the four points the user changed in the pointpick. then output the biezercurve then miku follow the curve to move,
void readfile(){


    float x_;
    float y_;
    float z_;
      ifstream myfile ("../pointpick/vertices.txt");

      if (myfile.is_open())
      {
        while (!myfile.eof() )
        {
            myfile >> x_;
            myfile >> y_;
            myfile >> z_;
//I get the value of one of the vec3 value
        vec3 point=vec3(x_, y_, z_);
          //cout << x_ << y_ << z_<< endl;

        //put it into the FourPoint vector
        FourPoint.push_back(point);
        }
        myfile.close();
      }

      else cout << "Unable to open file";

}


//use recursion to do the bezierCurve
void CreateBezierCurve(std::vector<vec3> input, std::vector<vec3> &BeizerCurve, float t, float increasing) {
    if(t>1)
        return;
    //all we want is a set of points contain the positon of the point in the curve
    //so input the four points, then output 1000 points, then use the methond given to draw the curve
    vec3 p13 = vec3(0,0,0);
    p13=(1-t)*(1-t)*(1-t)*input[0]+3*t*(1-t)*(1-t)*input[1]+3*t*t*(1-t)*input[2]+t*t*t*input[3];
    //beiercurve is a vector, can use push_back to put in vec3 values.
    BeizerCurve.push_back(p13);

    //recursion
    CreateBezierCurve(input,BeizerCurve,t+increasing, increasing);
}

void init(){
    glClearColor(/*white*/ 1.0f,1.0f,1.0f, /*solid*/1.0 );

//do textrue, apply the picture into the quad.
    foreground.init("money.tga");
    background.init("bbb.tga");
    GLuint fb_tex = fb.init();
    squad.init(fb_tex);

    miku_head.init("mikuhead.tga");
    hair.init("hair1.tga");
    hair_right.init("hair2.tga");
    hairBound1.init("hairBound.tga");
    hairBound2.init("hairBound.tga");
    body.init("body.tga");
    hand1.init("hand.tga");
    hand2.init("hand.tga");
    foot1.init("hand2.tga");
    foot2.init("hand2.tga");
    onion.init("food.tga");

    //init the biezer curve~~~
    t=0.001;
    increasing=0.001;
    //readfile input the four point
    readfile();
    CreateBezierCurve(FourPoint,BeizerCurve,t,increasing);
}

//use frameBuffer to do the "let it snow "part.
void drawScene(float timeCount)
{
    float time = glfwGetTime();
    glEnable(GL_BLEND);
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       //draw the background
       float t = timeCount * SPEED_FACTOR;
       Transform TRS = Transform::Identity();
       background.draw(TRS.matrix());
       float xcord = 0.7*std::cos(t);
       float ycord = 0.7*std::sin(t);


        //draw the snow
       //each snow is a quad, and has a white circle texture on it.
       Transform TRR1 = Transform::Identity();
       TRR1 *= Eigen::Translation3f(-1,1+fmod((0-(time/11)),2), 0);
       TRR1 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR1.matrix());

       Transform TRR2 = Transform::Identity();
       TRR2 *= Eigen::Translation3f(-0.8, 1+fmod((0-(time/6)),2), 0);
       TRR2 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR2.matrix());

       Transform TRR3 = Transform::Identity();
       TRR3 *= Eigen::Translation3f(-0.6, 1+fmod((0-(time/8)),2), 0);
       TRR3 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR3.matrix());

       Transform TRR4 = Transform::Identity();
       TRR4 *= Eigen::Translation3f(-0.4, 1+fmod((0-(time/9)),2), 0);
       TRR4 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR4.matrix());

       Transform TRR5 = Transform::Identity();
       TRR5 *= Eigen::Translation3f(-0.2, 1+fmod((0-(time/7)),2), 0);
       TRR5 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR5.matrix());

       Transform TRR6 = Transform::Identity();
       TRR6 *= Eigen::Translation3f(0, 1+fmod((0-(time/6)),2), 0);
       TRR6 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR6.matrix());

       Transform TRR7 = Transform::Identity();
       TRR7 *= Eigen::Translation3f(0.2, 1+fmod((0-(time/10)),2), 0);
       TRR7 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR7.matrix());

       Transform TRR8 = Transform::Identity();
       TRR8 *= Eigen::Translation3f(0.4, 1+fmod((0-(time/9)),2), 0);
       TRR8 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR8.matrix());

       Transform TRR9 = Transform::Identity();
       TRR9 *= Eigen::Translation3f(0.6, 1+fmod((0-(time/8)),2), 0);
       TRR9 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR9.matrix());

       Transform TRR10 = Transform::Identity();
       TRR10 *= Eigen::Translation3f(0.8, 1+fmod((0-(time/7)),2), 0);
       TRR10 *= Eigen::AlignedScaling3f(0.02, 0.02, 0.0);
       foreground.draw(TRR10.matrix());
//do it this amout of time, then the screen will be full of snow..





       glDisable(GL_BLEND);

}

void display(){

    //set up to draw stuff
    glClear(GL_COLOR_BUFFER_BIT);

    float time_s = glfwGetTime();
    float freq = M_PI*time_s*SPEED_FACTOR;

    /*begin of the frameBuffering*/
    opengp::update_title_fps("FrameBuffer");
    glViewport(0,0,width,height);
    fb.bind();
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    drawScene(glfwGetTime());
    //drawScene(glfwGetTime());
    fb.unbind();

    glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    squad.draw();


    /*begin of the miku*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//do move all , and apply move all to the whole miku , then the whole character will move.
    Transform move_all = Transform::Identity();
    float x_ =(0.7-fmod(0.1*time_s,1));
    float y_ =(0.5-fmod(0.1*time_s,1));

    //follow the biezercurve.
    move_all*= Eigen::Translation3f(BeizerCurve[(int)(fmod(time_s/10,1)*999)]);
    //move_all *= Eigen::Translation3f(x_, y_, 0.0);
    //miku will become bigger and bigger after time
    move_all *= Eigen::AlignedScaling3f(0.4+0.7*fmod(0.1*time_s,1),0.4+0.7*fmod(0.1*time_s,1),0.0);


//the foot
    Transform foot1_M=move_all;
    foot1_M*=Eigen::AlignedScaling3f(0.7,0.5,1.0);
    foot1_M*=Eigen::Translation3f(0.2,-1.7,0.0);
    foot1.draw(foot1_M.matrix());

    Transform foot2_M=move_all;
    foot2_M*=Eigen::AlignedScaling3f(0.7,0.5,1.0);
    foot2_M*=Eigen::Translation3f(-0.2,-1.7,0.0);
    foot2.draw(foot2_M.matrix());

    //the body
    Transform body_M=move_all;
    body_M*=Eigen::AlignedScaling3f(0.7,0.5,1.0);
    body_M*=Eigen::Translation3f(-0.1,-0.8,0.0);
   // body_M*=Eigen::AlignedScaling3f(0.7,0.5,1.0);
    body.draw(body_M.matrix());

    /*right hand*/
    Transform hand1_M=move_all;
    hand1_M*=Eigen::AlignedScaling3f(0.3,0.3,1.0);
    hand1_M*=Eigen::Translation3f(0.2,-0.5,0.0);
    hand1_M*=Eigen::AngleAxisf(M_PI*(0.5+-0.2*sin(time_s*9)) , Eigen::Vector3f::UnitZ());
    hand1_M*=Eigen::Translation3f(0,-1,0);
    hand1.draw(hand1_M.matrix());

    /*left hand*/
    Transform hand2_M=move_all;
    hand2_M*=Eigen::AlignedScaling3f(0.3,0.3,1.0);

    hand2_M*=Eigen::Translation3f(-0.9,-0.5,0.0);

    hand2_M*=Eigen::AngleAxisf(M_PI*(-0.5+0.2*sin(time_s*9)) , Eigen::Vector3f::UnitZ());
    hand2_M*=Eigen::Translation3f(0,-1,0.0);
    hand2.draw(hand2_M.matrix());

    /* Green Onion*/
    Transform onion_M=hand2_M;
   // onion_M*=Eigen::AlignedScaling3f(0.3,0.3,1.0);
    onion_M*=Eigen::Translation3f(0.5,-0.7,0.0);
    onion_M*=Eigen::AngleAxisf(time_s *4, Eigen::Vector3f::UnitZ());


    onion.draw(onion_M.matrix());





//the hair
    Transform hair_M=move_all;
    hair_M*=Eigen::Translation3f(-0.60,0.0,0.0);
    hair_M*=Eigen::AlignedScaling3f(0.3,0.4,1.0);
    hair_M *= Eigen::AngleAxisf(M_PI*0.1, Eigen::Vector3f::UnitZ());
    hair.draw(hair_M.matrix());

    Transform hair_right_M=move_all;
    hair_right_M*=Eigen::Translation3f(0.4,0.0,0.0);
    hair_right_M*=Eigen::AlignedScaling3f(0.3,0.4,1.0);
    hair_right_M *= Eigen::AngleAxisf(-M_PI*0.1, Eigen::Vector3f::UnitZ());
    hair_right.draw(hair_right_M.matrix());

//the head
    //this it the part I worked on first. so everyline has a comment on it.
    //The head of the miku
    Transform miku_head_M= move_all;
    //initial the matrix
    miku_head_M*=Eigen::Translation3f(-0.1,0.1,0.0);
    //position of the head
    float scale_u=0.01*std::sin(freq);
    miku_head_M*=Eigen::AlignedScaling3f(0.4+scale_u,0.25+scale_u,1.0);
    //make it smaller
    miku_head.draw(miku_head_M.matrix());

    //hairBound 1
    Transform hairBound1_M=move_all;
    hairBound1_M*=Eigen::Translation3f(-0.5,0.2,0.0);
    hairBound1_M*=Eigen::AlignedScaling3f(0.04,0.13,1.0);
    hairBound1.draw(hairBound1_M.matrix());
    //hairBound2
    Transform hairBound2_M=move_all;
    hairBound2_M*=Eigen::Translation3f(0.32,0.2,0.0);
    hairBound2_M*=Eigen::AlignedScaling3f(0.04,0.13,1.0);
    hairBound2.draw(hairBound2_M.matrix());


}


int main(int, char**){
    glfwInitWindowSize(700,700 );
    glfwCreateWindow("Miku");
    glfwDisplayFunc(display);

    init();
    readfile();

    glfwMainLoop();
    //triangle.cleanup();


    return EXIT_SUCCESS;

}
