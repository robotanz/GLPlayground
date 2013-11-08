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

using namespace std;

class ShaderProgram {
    
private:
    
    GLuint _programId;
    
    // base directory
    static string _baseDir;
    
    string _vsName;
    string _fsName;
    
    char * _errorLog;
    
    bool _status;
    
    string readFile(string const name);
    
    GLuint createShader(GLenum type, string const src);
    
    GLuint createProgram(string const vsName, string const fsName);
    
    // add standard uniforms
    
public:
    
    ShaderProgram(string const vsName, string const fsName);
    
    virtual ~ShaderProgram();
    
    static void setBaseDirectory(const string baseDir);
    
    bool isValid();
    
    char * getErrorLog();
    
    void useProgram();
    
    void stopProgram();
};


#endif /* defined(__GLPlayground__ShaderProgram__) */
