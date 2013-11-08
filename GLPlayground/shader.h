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

enum {
	ProgramLight,
	ProgramTexture,
	ProgramPostProcess,
	ProgramTessellation,
	PROGRAM_MAX
};

struct program{
	GLuint name;
	GLuint uniforms[UNIFORM_MAX];
    
	void get_uniforms(){
		uniforms[UniformTime] = glGetUniformLocation(name, "u_time"); // on recupere l'uniform du program
		uniforms[UniformMouse] = glGetUniformLocation(name, "u_mouse"); // on recupere l'uniform du program
		uniforms[UniformModelMatrix] = glGetUniformLocation(name, "u_model_matrix"); // on recupere l'uniform du program
		uniforms[UniformViewMatrix] = glGetUniformLocation(name, "u_view_matrix"); // on recupere l'uniform du program
		uniforms[UniformProjMatrix] = glGetUniformLocation(name, "u_proj_matrix"); // on recupere l'uniform du program
		uniforms[UniformLightTable] = glGetUniformLocation(name, "u_light"); // on recupere l'uniform du program. La chaine de caractere pointe sur le premier. Le reste est contigu.
		uniforms[UniformEye] = glGetUniformLocation(name, "u_eye"); // on recupere l'uniform du program
		uniforms[UniformTexture] = glGetUniformLocation(name, "u_texture"); // on recupere l'uniform du program
	}
    
	void set_uniforms(){
		// on donne une valeur differente de l'uniforme a chaque boucle
		glUniform1f(uniforms[UniformTime], get_time());
		glUniform2f(uniforms[UniformMouse], mouse.x, mouse.y);
		glUniformMatrix4fv(uniforms[UniformViewMatrix], 1, GL_FALSE, &view_matrix[0][0]); // param3 : stocker en ligne ou pas, ici pas besoin de transposer. param4 : pointeur vers les float de la matrice
		glUniformMatrix4fv(uniforms[UniformProjMatrix], 1, GL_FALSE, &proj_matrix[0][0]);
        
        
		glUniform4fv(uniforms[UniformLightTable], light_table.size(), &light_table[0][0]);
		vec3 eye = (inverse(view_matrix) * vec4(0.f, 0.f, 0.f, 1.f)).xyz();
		glUniform3fv(uniforms[UniformEye], 1, &eye[0]);
	}
};


// Note: free data afterwards.
char* loadFile(const char *filename, GLint *fSize){
	char * data;
	FILE *theFile;
	char c;
	long howMuch;
    
	// Get file length
	theFile = fopen(filename, "rb");
	if (theFile == NULL)
	{
		printf("%s not found\n", filename);
		return NULL;
	}
	howMuch = 0;
	c = 0;
	while (c != EOF)
	{
		c = getc(theFile);
		howMuch++;
	}
	fclose(theFile);
    
	printf("%ld bytes\n", howMuch);
    
	// Read it again
	data = (char *)malloc(howMuch);
	theFile = fopen(filename, "rb");
	fread(data, howMuch-1,1,theFile);
	fclose(theFile);
	data[howMuch-1] = 0;
    
	printf("\n--- Shader source %s ---\n%s\n-- end of %s --\n", filename, data, filename);
	printf("%s loaded from disk\n", filename);
	*fSize = (int)howMuch;
    
	return data;
}

static string read_file(string const name){
	ifstream file;
    
//    char *shaderPath = getenv("GL_PLAYGROUND_PATH");
//    if(!shaderPath) {
//        printf("GL_PLAYGROUND_PATH variable not defined. Cannot find shaders location!\n");
//        exit(1);
//    }
//    else {
//        printf("GL_PLAYGROUND_PATH defined: %s\n", shaderPath);
//    }
    
//    char filename[256];
//    strcpy(filename, shaderPath);
//    strcat(filename, "/");
//    strcat(filename, name.c_str());
    
//	file.open((string(shaderPath) + name).c_str());
	file.open((string("/Users/jgenti/Development/xcode-git/GLPlayground/GLPlayground/") + name).c_str());
	string text;
	file.seekg(0, ios::end);
    
    long size = file.tellg();
    
	text.resize(size); // tellg donne la taille du debut jusqu'au niveau du curseur
	file.seekg(0, ios::beg);
	file.read(&text[0], text.size());
	file.close();
    
    //free(shaderPath);
    
	return text;
}

/** 
* Creation du program
*/
static GLuint create_program(string const vert_shader, string const frag_shader, string const tc_shader = "", string const te_shader = "", string const geom_shader = ""){ // te = tessellation evaluation
	bool use_tess = /* geom_shader.size() != 0 */ false;
 
	string vert_code = read_file(vert_shader);
	string frag_code = read_file(frag_shader);
	string tc_code;
	string te_code;
	string geom_code;

	if(use_tess){
		tc_code = read_file(tc_shader);
		te_code = read_file(te_shader);
		geom_code = read_file(geom_shader);
	}

	GLuint vert_name = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_name = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint tc_name;
	GLuint te_name;
	GLuint geom_name;

	if(use_tess){
//		tc_name = glCreateShader(GL_TESS_CONTROL_SHADER);
//		te_name = glCreateShader(GL_TESS_EVALUATION_SHADER);
		geom_name = glCreateShader(GL_GEOMETRY_SHADER);
	}

	// 1) On envoi les codes sources aux shaders
	// vertex
	char const * str = vert_code.c_str();
	GLint sz = vert_code.size();
	glShaderSource(vert_name, 1, &str,&sz); // 2eme param : le nombre de fichier texte qu'on va lui donner.

	// fragment
	str = frag_code.c_str();
	sz = frag_code.size();
	glShaderSource(frag_name, 1, &str,&sz); 

	if(use_tess){
//		str = tc_code.c_str();
//		sz = tc_code.size();
//		glShaderSource(tc_name, 1, &str, &sz);
//
//		str = te_code.c_str();
//		sz = te_code.size();
//		glShaderSource(te_name, 1, &str, &sz);
//
//		str = geom_code.c_str();
//		sz = geom_code.size();
//		glShaderSource(geom_name, 1, &str, &sz);
	}

	// 2) on compile les shaders
	// vertex
	string errorlog;
	errorlog.resize(4096);
	glCompileShader(vert_name); // c'est le driver qui compile pour la carte video
	glGetShaderInfoLog(vert_name, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
	GLint status;
	glGetShaderiv(vert_name, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
	if (status != GL_TRUE){
		cerr << "Vertex Shader compilation failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}

	// tess
	if (use_tess){
		errorlog.resize(4096);
		glCompileShader(tc_name); // c'est le driver qui compile pour la carte video
		glGetShaderInfoLog(tc_name, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
		glGetShaderiv(tc_name, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
		if (status != GL_TRUE){
			cerr << "TC Shader compilation failure" << endl;
			errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
			cerr << errorlog;
			exit(1); // ca a merde, on quitte le programme
		}

		errorlog.resize(4096);
		glCompileShader(te_name); // c'est le driver qui compile pour la carte video
		glGetShaderInfoLog(te_name, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
		glGetShaderiv(te_name, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
		if (status != GL_TRUE){
			cerr << "TE Shader compilation failure" << endl;
			errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
			cerr << errorlog;
			exit(1); // ca a merde, on quitte le programme
		}

		errorlog.resize(4096);
		glCompileShader(geom_name); // c'est le driver qui compile pour la carte video
		glGetShaderInfoLog(geom_name, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
		glGetShaderiv(geom_name, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
		if (status != GL_TRUE){
			cerr << "Geom Shader compilation failure" << endl;
			errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
			cerr << errorlog;
			exit(1); // ca a merde, on quitte le programme
		}
	}

	// fragment
	errorlog.resize(4096);
	glCompileShader(frag_name); // c'est le driver qui compile pour la carte video
	glGetShaderInfoLog(frag_name, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
	glGetShaderiv(frag_name, GL_COMPILE_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
	if (status != GL_TRUE){
		cerr << "Fragment Shader compilation failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}

	// 4) on créé le program
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vert_name); // l'ordre d'ajout des shaders n'a pas d'importance
	glAttachShader(prog, frag_name);

	if (use_tess){
		glAttachShader(prog, tc_name);
		glAttachShader(prog, te_name);
		glAttachShader(prog, geom_name);
	}

	// 5) on link tout ca
	glLinkProgram(prog);
	glGetProgramInfoLog(prog, 4096, &sz, &errorlog[0]); // Donne un log de compilation. 2eme param : la taille du buffer dans lequel il a le droit d'ecrire. 3eme param : nombre de caractere qu'il a vraiment ecrit sur les 4096 total
	glGetProgramiv(prog, GL_LINK_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
	if (status != GL_TRUE){
		cerr << "Program link failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}

	// 6) validation du program
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status); // on demande des infos sur une certaine caracteristique pour savoir si la compil s'est bien passée.
	if (status != GL_TRUE){
		cerr << "Program validation failure" << endl;
		errorlog.resize(sz); // sz represente le nombre reel de caracteres ecrit dans errorlog
		cerr << errorlog;
		exit(1); // ca a merde, on quitte le programme
	}

	// 7) maintenant, les shaders ne nous interessent plus, on a uniquement besoin de connaitre le program
	glDeleteShader(vert_name);// detruira les shaders lorsqu'ils ne seront plus jamais utilises par aucun program
	glDeleteShader(frag_name);

	if (use_tess){
		glDeleteShader(tc_name);
		glDeleteShader(te_name);
		glDeleteShader(geom_name);
	}

	return prog;
}