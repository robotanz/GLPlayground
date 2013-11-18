//
//  Cube.h
//  GLPlayground
//
//  Created by Samuel Philipps on 15/11/2013.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#ifndef GLPlayground_Cube_h
#define GLPlayground_Cube_h

#include "Mesh.h"

class Cube : public Mesh {
    
public:
    
    Cube() : Mesh(){}
    
    void make(){
		// Chargement du model 3D
		// 1) Les vertex
		vertices.push_back(vertex(vec3(-1.f, 1.f, -1.0f), vec3(0.f,0.f,1.f)));
		vertices.push_back(vertex(vec3(-1.0f, -1.f, -1.0f), vec3(0.f, 0.f, 1.f)));
		vertices.push_back(vertex(vec3(1.f, 1.f, -1.0f), vec3(0.f, 0.f, 1.f)));
		vertices.push_back(vertex(vec3(1.f, -1.f, -1.0f), vec3(0.f, 0.f, 1.f)));
        vertices.push_back(vertex(vec3(1.f, -1.f, 1.0f), vec3(0.f, 0.f, 1.f)));
        vertices.push_back(vertex(vec3(-1.f, -1.f, 1.0f), vec3(0.f, 0.f, 1.f)));
        vertices.push_back(vertex(vec3(1.f, 1.f, 1.0f), vec3(0.f, 0.f, 1.f)));
        vertices.push_back(vertex(vec3(-1.f, 1.f, 1.0f), vec3(0.f, 0.f, 1.f)));
		// 2) la geometrie
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(0);
        indices.push_back(5);
        indices.push_back(1);
    }
    
    void updateMatrixModel(mat4 model_matrix, float time, vec2 mouse, GLuint uniform){
        model_matrix = translate(mat4(1.f), vec3(0.f, 0.5f + 1.f, 0.f));
        model_matrix = scale(model_matrix, vec3(0.4f));
        model_matrix = rotate(model_matrix, (-1) * time * 100.f, vec3(0.5f, 0.3f, -1.f));
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &model_matrix[0][0]);
    }
    
    void setSpecialParameters(){
        // Mode fil de fer
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    
};

#endif
