#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

//type define
typedef Eigen::ParametrizedLine<float, 3> ray3;

class Floor{
    private:
        vec3 normal_vector;
        vec3 position_vector;

    public:

        // constructor
        Floor(vec3 normal, vec3 position){
            this->normal_vector = normal.normalized();
            this->position_vector = position.normalized();
        }

        // destructor
        ~Floor(){}


        //same as sphere its checking if it hit the floor
        float floorIntersect(ray3 ray){
            vec3 n=this->normal_vector;
            vec3 o=ray.origin();
            vec3 d=ray.direction();
            float t= -((o-this->position_vector).dot(n))/(d.dot(n));
            return t;



        }

        vec3 getNormal(){
            return normal_vector;
        }






};
