#include "icg_common.h"

const static Scalar H = .7;
const static Scalar R = 2;

class MultiLine{
private:
    class Hull{
    public:
        Hull() {
            _p1 = _p2 = _p3 = _p4 = vec3::Zero();
        }

        vec3& p1(){ return _p1; }
        vec3& p2(){ return _p2; }
        vec3& p3(){ return _p3; }
        vec3& p4(){ return _p4; }

    private:
        vec3 _p1;
        vec3 _p2;
        vec3 _p3;
        vec3 _p4;
    };

private:
    Hull _hull;                  ///< control points
    std::vector<vec3> _vertices; ///< multiline points
    GLuint _vao;                 ///< Vertex array objects
    GLuint _pid;          ///< GLSL program ID
    GLuint _vbo;


private:

    void multiline(Hull & p)
    {
        _vertices.push_back(p.p1());
        _vertices.push_back(p.p2());
        _vertices.push_back(p.p3());
        _vertices.push_back(p.p4());
    }

public:
    void init(GLuint pid){
        ///--- Set the (compiled) shaders
        _pid = pid;

        /// Generate the vertex array
        glGenVertexArrays(ONE, &_vao);
        glBindVertexArray(_vao);

        /// Generate one buffer, put the resulting identifier in vertexbuffer
        glGenBuffers(ONE, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
    //the user can set the position of the four points
    void set_points(const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4) {
        _vertices.clear();

        ///--- initialize data
        _hull.p1() = p1;
        _hull.p2() = p2;
        _hull.p3() = p3;
        _hull.p4() = p4;

        ///--- create the multiline
        multiline(_hull);
    }

    //use recursion to do the bezierCurve
    void CreateBezierCurve(std::vector<vec3> input, std::vector<vec3> &BeizerCurve, float t, float increasing) {
        if(t>1)
            return;
        //all we want is a set of points contain the positon of the point in the curve
        //so input the four points, then output 1000 points, then use the methond given to draw the curve
        vec3 p13 = vec3(0,0,0);
        p13+=(1-t)*(1-t)*(1-t)*input[0];
        p13+=3*t*(1-t)*(1-t)*input[1];
        p13+=3*t*t*(1-t)*input[2]+t*t*t*input[3];
        //beiercurve is a vector, can use push_back to put in vec3 values.
        BeizerCurve.push_back(p13);

        //recursion
        CreateBezierCurve(input,BeizerCurve,t+increasing, increasing);
    }

    void draw(const mat4& model, const mat4& view, const mat4& projection){
        if (_vertices.empty()) return;

        //define the number of vertices in the curve
        float interve = 0.001;
        float t = 0.001;

        //create the curve!
        std::vector<vec3> BeizerCurve;

        CreateBezierCurve(_vertices, BeizerCurve, t ,interve );


        glUseProgram(_pid);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex Attribute ID for Vertex Positions
        GLuint position = glGetAttribLocation(_pid, "position");
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        ///--- vertices
        //change the buffer to applied on the beizercurve instead of the line given in lab
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*BeizerCurve.size(), &BeizerCurve[0], GL_STATIC_DRAW);

        ///--- setup view matrices
        GLuint projection_id = glGetUniformLocation(_pid, "projection");
        glUniformMatrix4fv(projection_id, ONE, DONT_TRANSPOSE, projection.data());
        mat4 MV = view*model;
        GLuint model_view_id = glGetUniformLocation(_pid, "model_view");
        glUniformMatrix4fv(model_view_id, ONE, DONT_TRANSPOSE, MV.data());
        check_error_gl();

        //this is the part I draw the biezerCurve
        glDrawArrays(GL_LINE_STRIP, 0, BeizerCurve.size());
        glDisableVertexAttribArray(position);
        glBindVertexArray(0);
        glUseProgram(0);
        check_error_gl();
    }
};
