//
//  ShaderProgram.cpp
//  GLPlayground
//
//  Created by Thomas Jgenti on 08/11/2013.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#include "ShaderProgram.h"
#include <fstream>

// base directory
string ShaderProgram::_baseDir = "";

string ShaderProgram::readFile(string const name) {
    
    ifstream file;
    
	file.open((_baseDir + name).c_str());
	string text;
	file.seekg(0, ios::end);
    text.resize(file.tellg()); // tellg donne la taille du debut jusqu'au niveau du curseur
	file.seekg(0, ios::beg);
	file.read(&text[0], text.size());
	file.close();
    
	return text;
}
    
GLuint ShaderProgram::createShader(GLenum type, string const src) {
    
    return 0;
}
    
GLuint ShaderProgram::createProgram(string const vsName, string const fsName) {
    
    return 0;
}
    
    // add standard uniforms
    
ShaderProgram::ShaderProgram(string const vsName, string const fsName) {
    
    _programId = 0;
}
    
ShaderProgram::~ShaderProgram() {
    
}
    
void ShaderProgram::setBaseDirectory(const string baseDir) {
    ShaderProgram::_baseDir = baseDir;
}
    
bool ShaderProgram::isValid() {
    return _status;
}
    
char * ShaderProgram::getErrorLog() {
    return _errorLog;
}
    
void ShaderProgram::useProgram() {
    glUseProgram(_programId);
}
    
void ShaderProgram::stopProgram() {
    
    glUseProgram(0);
}