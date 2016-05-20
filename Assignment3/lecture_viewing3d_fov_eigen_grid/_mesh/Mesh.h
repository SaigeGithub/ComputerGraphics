#pragma once
#include "icg_common.h"
#include "../perlin_noise.h"

class Mesh{
public:
    bool wireframe = false;
    
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID
    GLuint _tex; ///< Texture ID
    GLuint _snow; ///< Texture ID
    GLuint _grass; ///< Texture ID
    GLuint _sand; ///< Texture ID
    GLuint _water; ///< Texture ID
    GLuint _rock;


    GLuint _vpoint;    ///< memory buffer
    GLuint _vnormal;   ///< memory buffer
    std::vector<vec3> vpoints;
    std::vector<vec3> vnormals;
    std::vector<unsigned int> indices;
    GLuint RESTART_ID;
    GLuint _vbo_indices;
    float mesh_size;

public:        
    GLuint getProgramID(){ 
        return _pid; 
    }
    
    void init(){

        ///--- Compile the shaders
        _pid = opengp::load_shaders("_mesh/Mesh_vshader.glsl", "_mesh/Mesh_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);
        glUseProgram(_pid);
//put points into vector then upload into shader
        mesh_size=250;
        RESTART_ID=-100;
        float row_size= mesh_size*2+1;
        for(int i=-mesh_size; i<mesh_size+1; i++){
            for(int j=-mesh_size; j<mesh_size+1; j++){
                vpoints.push_back(vec3(j,0, i));
            }
        }

//upload your index
        for(int i=0; i<row_size-1; i++){
            for(int j=0; j<row_size; j++){
                indices.push_back(j+i*row_size);
                indices.push_back(j+(i+1)*row_size);

            }
            indices.push_back(RESTART_ID);
        }


        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        check_error_gl();                      
        

        ///--- Vertex Buffer
        glGenBuffers(ONE, &_vpoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vpoint);
        glBufferData(GL_ARRAY_BUFFER, vpoints.size() * sizeof(vec3), vpoints.data(), GL_STATIC_DRAW);
        check_error_gl();


        glGenBuffers(ONE, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        check_error_gl();        


        //generate perlin noise , and then upload into shaders.
        ///--- Change to our texture -> perlin noise
        perlin_noise perl;
        perl.base_init(1024);
        RGBImage P= perl.get_noise(512);
        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D , 0, GL_RGB, perl.get_width(), perl.get_height(), 0, GL_RGB, GL_FLOAT,P.data() );
        glUniform1i(glGetUniformLocation(_pid, "tex"), 0 /*GL_TEXTURE0*/);

        ///--- Load texture
        glGenTextures(1, &_grass);
        glBindTexture(GL_TEXTURE_2D, _grass);
        glfwLoadTexture2D("_mesh/grass.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glUniform1i(glGetUniformLocation(_pid, "grass"), 2 /*GL_TEXTURE0*/);

        glGenTextures(1, &_water);
        glBindTexture(GL_TEXTURE_2D, _water);
        glfwLoadTexture2D("_mesh/water.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glUniform1i(glGetUniformLocation(_pid, "water"), 3 /*GL_TEXTURE0*/);

        glGenTextures(1, &_snow);
        glBindTexture(GL_TEXTURE_2D, _snow);
        glfwLoadTexture2D("_mesh/snow.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glUniform1i(glGetUniformLocation(_pid, "snow"), 4 /*GL_TEXTURE0*/);

        glGenTextures(1, &_rock);
        glBindTexture(GL_TEXTURE_2D, _rock);
        glfwLoadTexture2D("_mesh/rock.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glUniform1i(glGetUniformLocation(_pid, "rock"), 5 /*GL_TEXTURE0*/);

        glGenTextures(1, &_sand);
        glBindTexture(GL_TEXTURE_2D, _sand);
        glfwLoadTexture2D("_mesh/sand.tga", 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glUniform1i(glGetUniformLocation(_pid, "sand"), 6 /*GL_TEXTURE0*/);
    }
           
    void cleanup(){
        /// TODO
    }

    void draw(){
        bindShader();  
            ///--- Setup the texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _tex);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, _grass);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, _water);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, _snow);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, _rock);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, _sand);
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(RESTART_ID);
            glUniform1f(glGetUniformLocation(_pid, "time"), glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            glDrawElements(GL_TRIANGLE_STRIP, /*#vertices*/ indices.size(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
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

        ///--- Vertex Attribute ID for Normals
        GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
        if (vnormal_id >= 0) {
            glEnableVertexAttribArray(vnormal_id);
            glBindBuffer(GL_ARRAY_BUFFER, _vnormal);
            glVertexAttribPointer(vnormal_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            check_error_gl();
        }
    }

    void unbindShader() {
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        if (vpoint_id >= 0)
            glDisableVertexAttribArray(vpoint_id);
        GLint vnormal_id = glGetAttribLocation(_pid, "vnormal");
        if (vnormal_id >= 0)
            glDisableVertexAttribArray(vnormal_id);
        glUseProgram(0);
        glBindVertexArray(0);
    }
};
