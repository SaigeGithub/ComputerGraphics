#include "icg_common.h"
#include <Eigen/Geometry>
#include "camera.h"
#include "sphere.h"
#include "imagePlane.h"
#include "Floor.h"


#ifndef WITH_OPENCV
#error OpenCV required for this exercise
#endif



typedef cv::Vec3b Colour;
Colour red() { return Colour(255, 0, 0); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

struct MyImage{
    /// Data (not private for convenience)
    int cols = 640;
    int rows = 480;
    ///  Channel with [0..255] range image (aka uchar)
    cv::Mat image = cv::Mat(rows, cols, CV_8UC3, cv::Scalar(255,255,255));

    Colour& operator()(int row, int col)
    {
        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);
        return image.at<cv::Vec3b>(cv::Point(col, row));
    }

    /// Displays the image and pauses program execution (until key pressed)
    void show(){
        /// Display the image
        cv::imshow("image", image);
        cv::waitKey(0);
    }

    /// Use files with "*.png" extension
    void save(const std::string& filename){
        cv::imwrite(filename, image);
    }
};

//define a light object, use it multiple times.
struct light{
    vec3 lightposition;
    float Ia;
    float Ip;
};

//I wanna do reflection. here is the function i get the colour of other objects and return back to the shpere.
//the ray I put in is the reflective out going ray..
//the out ray will trace other objects (the sphere itself will see)
Colour reflection(ray3 ray){

    sphere sph(vec3(0, 0, 2), 1.0f);
    Floor flo(vec3(0,1,0),vec3(0,-3,0));
    float d= flo.floorIntersect(ray);
    light light1;
    light1.lightposition=vec3(-4,3,-1);
    light1.Ia= 0.2f;
    light1.Ip= 0.8f;
    Colour LightKa(0,255,0);
    Colour LightKd(0,255,0);
    Colour LightKs(255,220,255);

    if(d>=0){
        vec3 l = vec3 (light1.lightposition-ray.pointAt(d)).normalized();

        vec3 v=(-1)*ray.direction();//.normalized();

        vec3 n = flo.getNormal();


        float difusion =n.dot(l);
        //n=n.normalized();
        vec3 r = 2*n*(n.dot(l))-l;
        float specular = pow(max(0.0f,v.dot(r)),3);
        vec3 checkS =light1.lightposition-ray.pointAt(d);
        ray3 newRay = ray3(ray.pointAt(d),checkS.normalized());
        float s= sph.intersectRay(newRay);

        //checkboard
        float xValue= ray.pointAt(d).x();
        float yValue= ray.pointAt(d).y();
        float zValue= ray.pointAt(d).z();
        float Si=sin(xValue)*sin(yValue)*sin(zValue);





        //check the floor,if it is checked, change the colour.
        if(Si>0){
            LightKa=black();
            LightKd=black();

        }else{
            LightKa=white();
            LightKd=white();
        }
        if(s!=-1){
            return light1.Ia*LightKa;
        }else
            return light1.Ia*LightKa+light1.Ip*(LightKd*difusion+LightKs*specular);
           // return lwe;
    }else

        return white();

}

//Most of my job is done in the main.
int main(int, char**){
    /// Rays and vectors represented with Eigen
    typedef Eigen::Vector3f vec3;
    typedef Eigen::ParametrizedLine<float, 3> ray3;
    
    MyImage image;
    
    /// TODO: define camera position and sphere position here
    ///
    //here is my sphere, set the center and the radius
    sphere sph(vec3(0, 0, 2), 1.0f);
    //set the camera
    camera cam(vec3(0, 0.5, -1));
    //set the floor position
    Floor flo(vec3(0,1,0),vec3(0,-3,0));
    //set where the users's eye is
    vec3 cameraPosition = vec3(0,0,-1);
    //Imageplane set up. use the code in lab2
    imagePlane plane(vec3(-1, -1, 0), vec3(1, 1, 0), image.rows, image.cols);
    //The light of the shpere,set up, colour and light postion stuff.
    light light1;
    light1.lightposition=vec3(-4,3,-1);
    light1.Ia= 0.2f;
    light1.Ip= 0.8f;


    //set up the colour of sphere
    Colour Ka(255,0,0);
    Colour Kd(255,0,0);//diffusion
    Colour Ks(255,255,255);
    //set up the colour of floor
    Colour LightKa(0,255,0);
    Colour LightKd(0,255,0);
    Colour LightKs(255,220,255);

    //now do the texture, first, import a image
    cv::Mat img = cv::imread( "/Users/saigee10/Desktop/try second time/raytrace/stars.jpg",1);


    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            /// TODO: build primary rays
           
            //generate the ray.
            vec3 pt = plane.generatePixelPos(row, col);
            ray3 ray = cam.generateRay(pt);
            //get the determinant of the sphere. see if there is a hit on the sphere.
            float t= sph.intersectRay(ray);
            //d is the determinnt of the floor
            float d= flo.floorIntersect(ray);
            //a,b ,c is used to do area lights
            float a=0.0f;
            float b=0.0f;
            float c=0.0f;
            //a for loop to do area lights.
            for(double i=-4; i<-2; i+=0.25){
                //change the position of the light
                light1.lightposition=vec3(i,3,-1);
                //if the ray hit on sphere
                if(t!=-1){
                    //Basiclly do a shpere
                    //image(row,col)=black();
                    //
                    //now the lights
                    vec3 l = vec3 (light1.lightposition-ray.pointAt(t)).normalized();

                    vec3 v=(-1)*ray.direction();//.normalized();

                    vec3 n = vec3 (ray.pointAt(t)-sph.getCenter());//.normalized();


                    float difusion =n.dot(l);
                    n=n.normalized();
                    vec3 r = 2*n*(n.dot(l))-l;
                    float specular = pow(max(0.0f,v.dot(r)),8);

                    //textrue, put an 2D image onto sphere
                    vec3 P=vec3( ray.pointAt(t));
                    vec3 C=vec3(sph.getCenter());
                    float pi= 3.1415926;
                    float phi= acos((P.z()-C.z())/sph.getRadius());
                    float theta=atan2(P.y()-C.y(),P.x()-C.x());
                    float U= phi/(2*pi);
                    float V=(pi-theta)/pi;
                    //cout<<img.at<Colour>((int)(V*img.rows),(int)(U*img.cols))<<endl;
                    Colour texture= img.at<Colour>((int)(U*img.cols),(int)(V*img.rows));
                    //do the reflection. create new ray
                    vec3 I= ray.direction().normalized();
                    vec3 O=I-2*(I.dot(n))*n;
                    //the new ray
                    ray3 Outray=ray3(ray.pointAt(t),O);
                    //pu into the function get the colour of reflection
                    Colour reflec= reflection(Outray);
                    Colour final=reflec*0.5f+texture*0.7f;


                    Colour spheres=light1.Ia*final+light1.Ip*(final*difusion+Ks*specular);
                    a=a+spheres[0];
                    b=b+spheres[1];
                    c=c+spheres[2];
                //if the ray hit on floor
                }else if(d>=0){
                    //kind of the same calculation of the sphere
                    vec3 l = vec3 (light1.lightposition-ray.pointAt(d)).normalized();

                    vec3 v=(-1)*ray.direction();//.normalized();

                    vec3 n = flo.getNormal();


                    float difusion =n.dot(l);

                    vec3 r = 2*n*(n.dot(l))-l;
                    float specular = pow(max(0.0f,v.dot(r)),3);
                    vec3 checkS =light1.lightposition-ray.pointAt(d);
                    ray3 newRay = ray3(ray.pointAt(d),checkS.normalized());
                    float s= sph.intersectRay(newRay);

                    //checkboard
                    float xValue= ray.pointAt(d).x();
                    float yValue= ray.pointAt(d).y();
                    float zValue= ray.pointAt(d).z();
                    float Si=sin(xValue)*sin(yValue)*sin(zValue);





                    //check the floor,if it is checked, change the colour.
                    if(Si>0){
                        LightKa=black();
                        LightKd=black();

                    }else{
                        LightKa=white();
                        LightKd=white();
                    }
                    //but if it hit the sphere ,it will give a shadow. so do the shadow first, then the checkboard(floor)
                    if(s!=-1){
                        Colour shadows=light1.Ia*LightKa;
                        a=a+shadows[0];
                        b=b+shadows[1];
                        c=c+shadows[2];

                    }else{
                        Colour checkboards= light1.Ia*LightKa+light1.Ip*(LightKd*difusion+LightKs*specular);
                        a=a+ checkboards[0];
                        b=b+ checkboards[1];
                        c=c+ checkboards[2];
                    }








                }else{
                    //anyways, it will give a white background!!!which is good!!
                    Colour backgroud =white();
                    a=a+backgroud[0];
                    b=b+backgroud[1];
                    c=c+backgroud[2];
                }



            }
            //here is also doing the area light
            Colour h;
            //check the colour to make it not "darker" than white
            if(a>255)
                 h[0]=255;

            if(b>255)
                 h[1]=255;
            if(c>255)
                 h[2]=255;
            a=a/8;
            b=b/8;
            c=c/8;
            //store the colour in h then return to the imageplane.
            h[0]=a;
             h[1]=b;
             h[2]=c;
             image(row,col)=h;


        }

    }


    
    image.show();
    // image.save("output.png"); ///< Does not work on Windows!

    return EXIT_SUCCESS;
}
