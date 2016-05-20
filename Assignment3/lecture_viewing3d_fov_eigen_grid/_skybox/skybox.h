#pragma once
#include "icg_common.h"


class skybox{
public:
    bool wireframe = false;
    
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID
    GLuint _tex; ///< Texture ID

//    GLuint textureID;



    GLuint _vpoint;    ///< memory buffer
   // GLuint _vnormal;   ///< memory buffer
    GLuint _vbo_indices;
    std::vector<vec3> vpoints;
    std::vector<unsigned int> indices;
   // std::vector<const GLchar*> faces;
    GLuint RESTART_IDD;
    float box_size;

public:        
    GLuint getProgramID(){ 
        return _pid; 
    }
 std::vector<vec3> put_points(){
        box_size=250;
        RESTART_IDD=-10;
        vpoints.push_back(vec3(-box_size,box_size+box_size,box_size));
        vpoints.push_back(vec3(box_size,box_size+box_size,box_size));
        vpoints.push_back(vec3(box_size,box_size+box_size,-box_size));
        vpoints.push_back(vec3(-box_size,box_size+box_size,-box_size));

        vpoints.push_back(vec3(-box_size,-box_size+box_size,box_size));
        vpoints.push_back(vec3(box_size,-box_size+box_size,box_size));
        vpoints.push_back(vec3(box_size,-box_size+box_size,-box_size));
        vpoints.push_back(vec3(-box_size,-box_size+box_size,-box_size));
        return vpoints;
    }

    std::vector<unsigned int> indexes(){

        RESTART_IDD=-10;

        indices.push_back(0);
        indices.push_back(4);
        indices.push_back(1);
        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(6);
        indices.push_back(3);
        indices.push_back(7);
        indices.push_back(0);
        indices.push_back(4);

        indices.push_back(RESTART_IDD);
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(RESTART_IDD);
        indices.push_back(7);
        indices.push_back(6);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(RESTART_IDD);
        return indices;
    }

    void init(){



         ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        check_error_gl();                      
        
        vpoints=put_points();
        ///--- Vertex Buffer
        glGenBuffers(ONE, &_vpoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glBufferData(GL_ARRAY_BUFFER, vpoints.size() * sizeof(vec3), vpoints.data(), GL_STATIC_DRAW);
        check_error_gl();


        indices=indexes();
        glGenBuffers(ONE, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        check_error_gl();        

        ///--- Compile the shaders
        _pid = opengp::load_shaders("_skybox/skybox_vshader.glsl", "_skybox/skybox_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);
        glUseProgram(_pid);
        


//        glGenTextures(1, &textureID);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//        GLFWimage right,left,top,bottom,front,back;
//        glfwReadImage("_skybox/right.tga",&right,GLFW_ORIGIN_UL_BIT); // flags?
//        glfwReadImage("_skybox/left.tga", &left, GLFW_ORIGIN_UL_BIT);
//        glfwReadImage("top.tga", &top, GLFW_ORIGIN_UL_BIT);
//        glfwReadImage("bottom.tga", &bottom, GLFW_ORIGIN_UL_BIT);
//        glfwReadImage("front.tga", &front, GLFW_ORIGIN_UL_BIT);
//        glfwReadImage("back.tga", &back, GLFW_ORIGIN_UL_BIT);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, right.Width, right.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, right.Data);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, left.Width, left.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, left.Data);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, top.Width, top.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, top.Data);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, bottom.Width, bottom.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom.Data);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, front.Width, front.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, front.Data);
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, back.Width, back.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, back.Data);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//        GLuint tex_id = glGetUniformLocation(_pid, "skybox_tex");
//                glUniform1i(tex_id, 6);
        //glUniform1f(glGetUniformLocation(_pid, "box_size"), (float)box_size);
       // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        GLFWimage right,left,top,bottom,front,back;
                glfwReadImage("_skybox/right.tga",&right,GLFW_ORIGIN_UL_BIT); // flags?
                glfwReadImage("_skybox/left.tga", &left, GLFW_ORIGIN_UL_BIT);
                glfwReadImage("_skybox/top.tga", &top, GLFW_ORIGIN_UL_BIT);
                glfwReadImage("_skybox/bottom.tga", &bottom, GLFW_ORIGIN_UL_BIT);
                glfwReadImage("_skybox/front.tga", &front, GLFW_ORIGIN_UL_BIT);
                glfwReadImage("_skybox/back.tga", &back, GLFW_ORIGIN_UL_BIT);

                glGenTextures(1, &_tex);
                glBindTexture(GL_TEXTURE_CUBE_MAP, _tex);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, right.Width, right.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, right.Data);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, left.Width, left.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, left.Data);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top.Width, top.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, top.Data);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottom.Width, bottom.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom.Data);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, front.Width, front.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, front.Data);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, back.Width, back.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, back.Data);
                GLuint tex_id = glGetUniformLocation(_pid, "skybox_tex");
                glUniform1i(tex_id, 6);


    }
           
    void cleanup(){
        /// TODO
    }

    void draw(){
        bindShader();
                    ///--- Setup the texture units
                    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); //seamless cube
                    glActiveTexture(GL_TEXTURE6);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, _tex);
                    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
                    glEnable(GL_PRIMITIVE_RESTART); // restart
                    glPrimitiveRestartIndex(RESTART_IDD);
                    glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
                unbindShader();
    }
    
private:
    void bindShader() {
        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Attribute ID for Positions
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        if (vpoint_id >= 0) {
            glEnableVertexAttribArray(vpoint_id);
            check_error_gl();
            glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
            glVertexAttribPointer(vpoint_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            check_error_gl();
        }


    }

    void unbindShader() {
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        if (vpoint_id >= 0)
            glDisableVertexAttribArray(vpoint_id);
        glUseProgram(0);
        glBindVertexArray(0);
    }
};
