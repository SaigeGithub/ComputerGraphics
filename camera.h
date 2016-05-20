#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

class camera{
public:
    // We only need the position of the camera.
    camera(vec3 const& eye) : mEye(eye) { }

    // Given coordinates of a pixel in the view plane, we generate its primary ray
    inline Eigen::ParametrizedLine<float, 3> generateRay(vec3 const& pt){
        typedef Eigen::ParametrizedLine<float, 3> ray3;

        vec3 origin = mEye;
        vec3 direction = (pt-mEye).normalized();
        return ray3(origin, direction);
    }

private:
    vec3 mEye; // The position of the camera.
};
