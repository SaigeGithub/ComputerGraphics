#pragma once
#include "icg_common.h"

class imagePlane{
public:
    imagePlane(vec3 const& llCorner, // The lower-left corner (l, b, 0)
               vec3 const& urCorner, // The upper-right corner (r, t, 0)
               int const& xRes, int const& yRes) : // The dimensions of the image
        l(llCorner.x()),
        b(llCorner.y()),
        r(urCorner.x()),
        t(urCorner.y()),
        mRows(xRes),
        mCols(yRes)
    { }

    inline vec3 generatePixelPos(int i, int j){
        float u = l + (r - l) * (i + 0.5f) / mRows;
        float v = (4.0f/3.0f)*b + (4.0f/3.0f)*(t - b) * (j + 0.5f) / mCols;
        return vec3(v, -u, 0);
    }

private:
    int mRows, mCols;
    float l, r, b, t;
};
