#pragma once
#include "icg_common.h"

//#include "OpenGLImage/EigenVisualizer.h"
//using namespace EigenVisualizer;

typedef Eigen::Matrix<Eigen::Vector3f, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RGBImage;

class perlin_noise{
public:
    float time = glfwGetTime();
    RGBImage base;

    void base_init(int size){
        std::srand(1);
        this->base = RGBImage(size,size);
        for (int i = 0; i < size; ++ i)
            for (int j = 0; j < size; ++ j)
            {
                vec3 randGradientVec;
                //we use vec3 instead of vec2 but set the last component to zero
                randGradientVec(0) = cos(2 * M_PI * rand0_1());
                randGradientVec(1) = sin(2 * M_PI * rand0_1());
                randGradientVec(2) = 0;
                this->base(i, j) = randGradientVec;
            }
    }

    float rand0_1()
    {
        return ((float) std::rand())/((float) RAND_MAX);
    }

    float mix(float x, float y, float alpha)
    {
        return y * alpha + x * (1.0f - alpha);
    }

    float f(float t)
    {
        float t_3 = t * t * t;
        return 6 * t * t * t_3 - 15 * t * t_3 + 10 * t_3;
    }

//I calculate the fBm from every point, and then output a vec3.
    vec3 get_noise_point(int x, int y, int period){
        float value= 0.0;
        float h= 1.0;
        float octaves =8;
        float l= 2.0;
        for(int i= 0; i< octaves; i++){
            float frequency = 1.0f / period;

            int left = (x / period) * period;
            int right = (left + period) % width;
            float dx = (x - left) * frequency;

            int top = (y / period) * period;
            int bottom = (top + period) % height;
            float dy = (y - top) * frequency;

            vec2 a(dx, -dy);
            vec2 b(dx - 1, -dy);
            vec2 c(dx - 1, 1 - dy);
            vec2 d(dx, 1 - dy);

            vec3 topleft = base(left, top);
            float s = topleft(0) * a(0) + topleft(1) * a(1);
            vec3 topright = base(right, top);
            float t = topright(0) * b(0) + topright(1) * b(1);
            vec3 bottomleft = base(left, bottom);
            float u = bottomleft(0) * d(0) + bottomleft(1) * d(1);
            vec3 bottomright = base(right, bottom);
            float v = bottomright(0) * c(0) + bottomright(1) * c(1);

            float fx = f(dx);
            float fy = f(dy);

            float st = mix(s, t, fx);
            float uv = mix(u, v, fx);
            float noise = mix(st, uv, fy);


            value += noise*pow(l, -h*i);
            period /= l;
            frequency = 1.0f / period;
        }


        return vec3(value, value, value) ;
    }
//store each point's value into a whole RGBimage and then upload into shaders..
    RGBImage get_noise(int period){

           RGBImage PerlinNoise(width, height);
           for (int i = 0; i < width; ++ i)
               for (int j = 0; j < height; ++ j)
               {
                   PerlinNoise(i, j) = get_noise_point(i,j,period) ;
               }
        return PerlinNoise;
    }



    int get_width(){
        return this->width;
    }
    int get_height(){
        return this->height;
    }

private:
    int width = 512;
    int height = 512;
};
