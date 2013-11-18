//
//  Ground.h
//  GLPlayground
//
//  Created by Samuel Philipps on 15/11/2013.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#ifndef GLPlayground_Ground_h
#define GLPlayground_Ground_h

#include "Mesh.h"

class Ground : public Mesh {
    
public:
    
    Ground() : Mesh(){}
    
    void make(){
        // Chargement du model 3D
		// 1) Les vertex
		vertices.push_back(vertex(vec3(-1.f, 1.f, 0.0f), vec3(0.f,0.f,1.f)));
		vertices.push_back(vertex(vec3(-1.0f, -1.f, 0.0f), vec3(0.f, 0.f, 1.f)));
		vertices.push_back(vertex(vec3(1.f, 1.f, 0.0f), vec3(0.f, 0.f, 1.f)));
		vertices.push_back(vertex(vec3(1.f, -1.f, 0.0f), vec3(0.f, 0.f, 1.f)));
		// 2) la geometrie
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
    }
    
    void updateMatrixModel(mat4 model_matrix, float time, vec2 mouse, GLuint uniform){
        model_matrix = scale(mat4(1.f), vec3(100.f));
        model_matrix = rotate(model_matrix, -90.f, vec3(1.f, 0.f, 0.f));
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &model_matrix[0][0]);
    }
    
};


#endif
