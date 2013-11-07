struct vertex{
	vec3 pos;
	vec3 norm; 

	vertex(vec3 in_pos, vec3 in_norm) : pos(in_pos), norm(in_norm){}
};

struct mesh{

	mesh(int i_current_VAO){
        // On sauvegarde le VAO correspondant a ce mesh
        index_current_VAO = i_current_VAO;
		
        // generation des noms de buffers
		glGenBuffers(NB_VBO,buffers);
	}

	void upload(){
		bind();
		// on donne les données
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW); // la taille doit être donnée en octet (d'où le "sizeof")
		// on donne les données
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW); // la taille doit être donnée en octet (d'où le "sizeof")
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
		glBindBuffer(GL_ARRAY_BUFFER, 0); // on rend le buffer inactif. Si plus tard on veut y retoucher, il suffira juste de le re rendre actif. Comme ça on évite de taper dedans sans faire expres.
		// on dit qu'on a plus besoin du buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // on rend le buffer inactif. Si plus tard on veut y retoucher, il suffira juste de le re rendre actif. Comme ça on évite de taper dedans sans faire expres.
        
        glBindVertexArray(0);
         
	}

	void draw(GLuint program){
		// FIXME : ce code devrait etre dans une classe "shader"
		GLuint attributes[2]; // l'attribut "in" que l'on se trimballe d'un shader a l'autre
		
        attributes[0] = glGetAttribLocation(program,"in_vertex"); // on recupere l'attribut du program
		attributes[1] = glGetAttribLocation(program,"in_norm"); // on recupere l'attribut du program
        
		// on active l'attribut (au niveau de la variable du shader)
		glEnableVertexAttribArray(attributes[0]);
		glEnableVertexAttribArray(attributes[1]);
		// on lie les attributs au vertex buffer. Il faut préciser le type de données que contiennent les buffers.
		glVertexAttribPointer(attributes[0], 3, GL_FLOAT, GL_FALSE, sizeof(vertex), NULL);// param 1 : le nom de l'attribut qu'on vient de recuperer. param 4 : nombre d'éléments * sa taille avant la prochaine donnee. un appel de cette fonction par variable
		glVertexAttribPointer(attributes[1], 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)sizeof(vec3));// param 1 : le nom de l'attribut qu'on vient de recuperer. param 4 : nombre d'éléments * sa taille avant la prochaine donnee. param 5 : valeur de depart. un appel de cette fonction par variable
		// maintenant le program sait ce qu'il doit aller chercher dans le buffer

		// on specifie au program sous quelle forme sont les indices (TRIANGLE, STRIP, FAN, ...)
		glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, NULL); // param 2 : nb de vertex qui construiront des primitives. param 3 : le type. param 4 : pointeur vers le debut des indices

        
		// on nettoie
		glDisableVertexAttribArray(attributes[0]);
		glDisableVertexAttribArray(attributes[1]);
 
	}

	void make_quad(){
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

	void make_torus(float r, int n1, int n2){
		// Chargement du model 3D
		// 1) Les vertex
		float pi = acos(-1.f);
		float r0 = 0.5f * (1.f + r); // le centre de la bouee (des cercles que l'on construit pour faire la bouee)
		float r1 = 0.5f * (1.f - r); // le rayon de la bouee
		for (int j = 0 ; j < n2 ; ++j){ // pour chaque cercle du tore
			float alpha = 2.f * pi * j / n2;
			vec3 u( cos(alpha), sin(alpha), 0.f);
			vec3 v(0.f, 0.f, 1.f);
			vec3 center = r0 * u;
			for (int i = 0 ; i < n1 ; ++i){
				float beta = 2.f * pi * i / n1; // angle de chaque cercle
				vec3 normal = u * cos(beta) + v * sin(beta);
				vec3 coordonnee = center + r1 * normal;

				vertices.push_back(vertex(coordonnee, - normal));
			}
		}
		// 2) la geometrie
		for (int j = 0 ; j < n2 ; ++j){ // pour chaque cercle du tore
			int a = j * n1;
			int b = (j + 1) % n2 * n1;

			indices.push_back(a); // pour recommencer le triangle strip (ajout d'un triangle degenere)
			indices.push_back(a);
			indices.push_back(b);
			
			for (int i = 0 ; i < n1 ; ++i){
				int c = j * n1 + (i + 1) % n1;
				int d = (j + 1) % n2 * n1 + (i + 1) % n1;
				
				indices.push_back(c);
				indices.push_back(d);
				
				// on passe aux deux triangles suivants
				a = c;
				b = d;
			}
			indices.push_back(b); // pour recommencer le triangle strip (ajout d'un triangle degenere)
		}
	}

	void make_pyramid(){
		// Chargement du model 3D
		
		vec3 p0 = vec3(0.0, 0.0, 0.0);
		vec3 p1 = vec3(0.0, 0.0, 0.0);
		vec3 p2 = vec3(0.0, 0.0, 0.0);
		vec3 p3 = vec3(0.0, 0.0, 0.0);
		vertices.push_back(vertex(p0, p0));
		vertices.push_back(vertex(p1, p1));
		vertices.push_back(vertex(p2, p2));
		vertices.push_back(vertex(p3, p3));
		

		// 1) on charge uniquement les indices. Les vertex seront crees dans le vertex shader
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(1);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
	}

	vector<vertex> vertices; // l'interet d'utiliser "vector" est que toutes ses données sont contigues en mémoire. Alors que dans une liste, les données peuvent être n'importe où (liées par pointeur)
	vector<int> indices; // geometrie
    
    int index_current_VAO;
    
    // VBO
	GLuint buffers[NB_VBO];

};