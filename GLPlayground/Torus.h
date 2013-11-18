//
//  Torus.h
//  GLPlayground
//
//  Created by Samuel Philipps on 15/11/2013.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#ifndef GLPlayground_Torus_h
#define GLPlayground_Torus_h

#include "Mesh.h"

class Torus : public Mesh {
    
private:
    
    float _r;
    int _n1;
    int _n2;
    
public:
    
    Torus(float r, int n1, int n2) : _r(r), _n1(n1), _n2(n2) {}
    
    void make(){
		// Chargement du model 3D
		// 1) Les vertex
		float pi = acos(-1.f);
		float r0 = 0.5f * (1.f + _r); // le centre de la bouee (des cercles que l'on construit pour faire la bouee)
		float r1 = 0.5f * (1.f - _r); // le rayon de la bouee
		for (int j = 0 ; j < _n2 ; ++j){ // pour chaque cercle du tore
			float alpha = 2.f * pi * j / _n2;
			vec3 u( cos(alpha), sin(alpha), 0.f);
			vec3 v(0.f, 0.f, 1.f);
			vec3 center = r0 * u;
			for (int i = 0 ; i < _n1 ; ++i){
				float beta = 2.f * pi * i / _n1; // angle de chaque cercle
				vec3 normal = u * cos(beta) + v * sin(beta);
				vec3 coordonnee = center + r1 * normal;
                
				vertices.push_back(vertex(coordonnee, - normal));
			}
		}
		// 2) la geometrie
		for (int j = 0 ; j < _n2 ; ++j){ // pour chaque cercle du tore
			int a = j * _n1;
			int b = (j + 1) % _n2 * _n1;
            
			indices.push_back(a); // pour recommencer le triangle strip (ajout d'un triangle degenere)
			indices.push_back(a);
			indices.push_back(b);
			
			for (int i = 0 ; i < _n1 ; ++i){
				int c = j * _n1 + (i + 1) % _n1;
				int d = (j + 1) % _n2 * _n1 + (i + 1) % _n1;
				
				indices.push_back(c);
				indices.push_back(d);
				
				// on passe aux deux triangles suivants
				a = c;
				b = d;
			}
			indices.push_back(b); // pour recommencer le triangle strip (ajout d'un triangle degenere)
		}
    }
    
    void updateMatrixModel(mat4 model_matrix, float time, vec2 mouse, GLuint uniform){
        model_matrix = translate(mat4(1.f), vec3(0.f, 0.5f + 1.f, 0.f));
        model_matrix = scale(model_matrix, vec3(0.30f));
        model_matrix = rotate(model_matrix, time * 100.f, vec3(1.f, 1.f, 0.f));
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &model_matrix[0][0]);
    }
    
};

#endif
