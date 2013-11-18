//
//  ShaderProgram.h
//  GLPlayground
//
//  Created by Thomas Jgenti on 08/11/2013.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#ifndef __GLPlayground__ShaderProgram__
#define __GLPlayground__ShaderProgram__

#include <iostream>
#include <OpenGL/gl3.h>

#include <vector>
using namespace std;

#define GLM_SWIZZLE 1
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/swizzle.hpp"
using namespace glm;

using namespace std;

class ShaderProgram {

public:
    
    enum {
        UniformTime,
        UniformMouse,
        UniformModelMatrix,
        UniformViewMatrix,
        UniformProjMatrix,
        UniformEye,
        UniformLightTable,
        UniformTexture,
        UNIFORM_MAX
    };
    
    ShaderProgram(string const vsName, string const fsName);
    
    virtual ~ShaderProgram();
    
    static void setBaseDirectory(const string baseDir);
    
    bool isValid();
    
    char* getErrorLog();
    
    void useProgram();
    
    void stopProgram();
    
    GLuint getAttribLocation(const string attribute);
    
    GLuint getUniform(const int uniformIndex);
    
    void sendUniforms(float time, vec2 mouse, mat4 view_matrix, mat4 proj_matrix, vector<vec4> light_table);
    
private:
    
    GLuint _programId;
    
    // base directory
    static string _baseDir;
    
    string _vsName, _fsName;
    
    char * _errorLog;
    
    bool _status;
    
    GLuint uniforms[UNIFORM_MAX];
    
    string readFile(string const name);
    
    GLuint createShader(GLenum type, string const src);
    
    GLuint createProgram(string const vsName, string const fsName);
    
    void fillUniforms();

};


#endif /* defined(__GLPlayground__ShaderProgram__) */
