#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

class sphere{
    typedef Eigen::Vector3f vec3;
    typedef Eigen::ParametrizedLine<float, 3> ray3;
public:
    sphere(vec3 const& c, float r) :
        mCentre(c),
        mRadius(r)
    { }
    //check if it hit the sphere
    float intersectRay(ray3 ray){
        float t;
        vec3 c= mCentre;
        vec3 o= ray.origin();
        vec3 d=ray.direction();
        float A=d.dot(d);
        float B=2*d.dot(o-c);
        float C=c.dot(c)+o.dot(o)-2*c.dot(o)-mRadius*mRadius;
        float discriminant=B*B-4*A*C;
        if((discriminant<0)||(discriminant==0)){
            t=-1;
        }else
            //t value is the point the ray hit the sphere
            t=(-B-sqrt(discriminant))/(2*A);
        return t;
    }
    vec3 getCenter(){

      return mCentre;
    }

    float getRadius(){

        return mRadius;
    };

private:
    float mRadius;
    vec3 mCentre, mNormal;
};
