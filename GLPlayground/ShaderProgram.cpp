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
    
    // + operator seems to have a bug or uses a wrong encoding. append works fine
    string path(_baseDir); // duplicate base bundle resources path
    path.append(name); // append shader's name
    const char* str = path.c_str(); // get the internal c string
    //printf("Shader Path: %s\n", str);
    
	file.open(str);
	file.seekg(0, ios::end);
    
    long size = file.tellg();
    
	string text;
	text.resize(size); // tellg donne la taille du debut jusqu'au niveau du curseur
	file.seekg(0, ios::beg);
	file.read(&text[0], text.size());
    
	file.close();
    
	return text;
}

GLuint ShaderProgram::createShader(GLenum type, string const src) {
    
    return 0;
}

GLuint ShaderProgram::createProgram(string const vsName, string const fsName) {
	string vertCode = readFile(vsName);
	string fragCode = readFile(fsName);
    
	GLuint vertName = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragName = glCreateShader(GL_FRAGMENT_SHADER);
    
	// 1) On envoi les codes sources aux shaders
	// vertex
	char const * str = vertCode.c_str();
	GLint sz = (GLint)vertCode.size();
	glShaderSource(vertName, 1, &str,&sz); // 2eme param : le nombre de fichier texte qu'on va lui donner.
    
	// fragment
	str = fragCode.c_str();
	sz = (GLint)fragCode.size();
	glShaderSource(fragName, 1, &str,&sz);
    
	// 2) on compile les shaders
	// vertex
	string errorlog;
	errorlog.resize(4096);
	glCompileShader(vertName); // c'est le driver qui compile pour la carte video
	glGetShaderInfoLog(vertName, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
	GLint status;
	glGetShaderiv(vertName, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passÈe.
	if (status != GL_TRUE){
		cerr << "Vertex Shader compilation failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}
    
	// fragment
	errorlog.resize(4096);
	glCompileShader(fragName); // c'est le driver qui compile pour la carte video
	glGetShaderInfoLog(fragName, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
	glGetShaderiv(fragName, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passÈe.
	if (status != GL_TRUE){
		cerr << "Fragment Shader compilation failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}
    
	// 4) on crÈÈ le program
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vertName); // l'ordre d'ajout des shaders n'a pas d'importance
	glAttachShader(prog, fragName);
    
	// 5) on link tout ca
	glLinkProgram(prog);
	glGetProgramInfoLog(prog, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
	glGetProgramiv(prog, GL_LINK_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passÈe.
	if (status != GL_TRUE){
		cerr << "Program link failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}
    
	// 6) validation du program
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passÈe.
	if (status != GL_TRUE){
		cerr << "Program validation failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}
    
	// 7) maintenant, les shaders ne nous interessent plus, on a uniquement besoin de connaitre le program
	glDeleteShader(vertName);// detruira les shaders lorsqu'ils ne seront plus jamais utilises par aucun program
	glDeleteShader(fragName);
    
	return prog;
}

// add standard uniforms

ShaderProgram::ShaderProgram(string const vsName, string const fsName) {
    _programId = createProgram(vsName, fsName);
    fillUniforms();
}

ShaderProgram::~ShaderProgram() {
    
}

void ShaderProgram::fillUniforms() {
    uniforms[UniformTime] = glGetUniformLocation(_programId, "u_time"); // on recupere l'uniform du program
    uniforms[UniformMouse] = glGetUniformLocation(_programId, "u_mouse"); // on recupere l'uniform du program
    uniforms[UniformModelMatrix] = glGetUniformLocation(_programId, "u_model_matrix"); // on recupere l'uniform du program
    uniforms[UniformViewMatrix] = glGetUniformLocation(_programId, "u_view_matrix"); // on recupere l'uniform du program
    uniforms[UniformProjMatrix] = glGetUniformLocation(_programId, "u_proj_matrix"); // on recupere l'uniform du program
    uniforms[UniformLightTable] = glGetUniformLocation(_programId, "u_light"); // on recupere l'uniform du program. La chaine de caractere pointe sur le premier. Le reste est contigu.
    uniforms[UniformEye] = glGetUniformLocation(_programId, "u_eye"); // on recupere l'uniform du program
    uniforms[UniformTexture] = glGetUniformLocation(_programId, "u_texture"); // on recupere l'uniform du program
}

void ShaderProgram::sendUniforms(float time, vec2 mouse, mat4 view_matrix, mat4 proj_matrix, vector<vec4> light_table) {
    // on donne une valeur differente de l'uniforme a chaque boucle
    glUniform1f(uniforms[UniformTime], time);
    glUniform2f(uniforms[UniformMouse], mouse.x, mouse.y);
    glUniformMatrix4fv(uniforms[UniformViewMatrix], 1, GL_FALSE, &view_matrix[0][0]); // param3 : stocker en ligne ou pas, ici pas besoin de transposer. param4 : pointeur vers les float de la matrice
    glUniformMatrix4fv(uniforms[UniformProjMatrix], 1, GL_FALSE, &proj_matrix[0][0]);
    
    
    glUniform4fv(uniforms[UniformLightTable], light_table.size(), &light_table[0][0]);
    vec3 eye = (inverse(view_matrix) * vec4(0.f, 0.f, 0.f, 1.f)).xyz();
    glUniform3fv(uniforms[UniformEye], 1, &eye[0]);
}

GLuint ShaderProgram::getUniform(const int uniformIndex){
    return uniforms[uniformIndex];
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

GLuint ShaderProgram::getAttribLocation(const string attribute){
    return glGetAttribLocation(_programId,attribute.c_str());
}