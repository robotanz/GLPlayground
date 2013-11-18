//
//  Mesh.h
//  GLPlayground
//
//  Created by Samuel Philipps on 15/11/2013.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#ifndef GLPlayground_Mesh_h
#define GLPlayground_Mesh_h

int VAO_id = 0;

struct vertex{
	vec3 pos;
	vec3 norm;
    
	vertex(vec3 in_pos, vec3 in_norm) : pos(in_pos), norm(in_norm){}
};

class Mesh{
    
    public :
    
	Mesh(){
        // On sauvegarde le VAO correspondant a ce mesh
        index_current_VAO = VAO_id++;
		
        // generation des noms de buffers
		glGenBuffers(NB_VBO,buffers);
	}
    
	void upload(){
		bind();
		// on donne les donnÈes
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW); // la taille doit Ítre donnÈe en octet (d'o˘ le "sizeof")
		// on donne les donnÈes
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW); // la taille doit Ítre donnÈe en octet (d'o˘ le "sizeof")
		unbind();
	}
    
	void bind(){
        // Setup first Vertex Array Object
        glBindVertexArray(vertexArrayObjID[index_current_VAO]);
        
		// on bind le buffer des vertex
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]); // dit que le buffer actif est le buffer triangle[0]
		// on bind le buffer des indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]); // dit que le buffer actif est le buffer triangle[1]
	}
    
	void unbind(){
        // on dit qu'on a plus besoin du buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0); // on rend le buffer inactif. Si plus tard on veut y retoucher, il suffira juste de le re rendre actif. Comme Áa on Èvite de taper dedans sans faire expres.
		// on dit qu'on a plus besoin du buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // on rend le buffer inactif. Si plus tard on veut y retoucher, il suffira juste de le re rendre actif. Comme Áa on Èvite de taper dedans sans faire expres.
        
        glBindVertexArray(0);
	}
    
	void draw(ShaderProgram* program){
		// FIXME : ce code devrait etre dans une classe "shader"
		GLuint attributes[2]; // l'attribut "in" que l'on se trimballe d'un shader a l'autre
        
        attributes[0] = program->getAttribLocation("in_vertex"); // on recupere l'attribut du program
		attributes[1] = program->getAttribLocation("in_norm"); // on recupere l'attribut du program
        
		// on active l'attribut (au niveau de la variable du shader)
		glEnableVertexAttribArray(attributes[0]);
		glEnableVertexAttribArray(attributes[1]);
		// on lie les attributs au vertex buffer. Il faut prÈciser le type de donnÈes que contiennent les buffers.
		glVertexAttribPointer(attributes[0], 3, GL_FLOAT, GL_FALSE, sizeof(vertex), NULL);// param 1 : le nom de l'attribut qu'on vient de recuperer. param 4 : nombre d'ÈlÈments * sa taille avant la prochaine donnee. un appel de cette fonction par variable
		glVertexAttribPointer(attributes[1], 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)sizeof(vec3));// param 1 : le nom de l'attribut qu'on vient de recuperer. param 4 : nombre d'ÈlÈments * sa taille avant la prochaine donnee. param 5 : valeur de depart. un appel de cette fonction par variable
		// maintenant le program sait ce qu'il doit aller chercher dans le buffer
        
		// on specifie au program sous quelle forme sont les indices (TRIANGLE, STRIP, FAN, ...)
		glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, NULL); // param 2 : nb de vertex qui construiront des primitives. param 3 : le type. param 4 : pointeur vers le debut des indices
        
		// on nettoie
		glDisableVertexAttribArray(attributes[0]);
		glDisableVertexAttribArray(attributes[1]);
	}
    
    virtual void make() = 0;
    
    // Pour l'instant, inutile de passer la "model_matrix" en param. Mais pourrait être utile plus tard
    virtual void updateMatrixModel(mat4 model_matrix, float time, vec2 mouse, GLuint uniform) = 0;
    
    virtual void setSpecialParameters(){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    
	vector<vertex> vertices; // l'interet d'utiliser "vector" est que toutes ses donnÈes sont contigues en mÈmoire. Alors que dans une liste, les donnÈes peuvent Ítre n'importe o˘ (liÈes par pointeur)
	vector<int> indices; // geometrie
    
    int index_current_VAO;
    
    // VBO
	GLuint buffers[NB_VBO];
    
};

#endif
