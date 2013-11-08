//
//  utilitaire.h
//  GLPlayground
//
//  Created by Samuel Philipps on 06/11/13.
//  Copyright (c) 2013 Thomas Jgenti. All rights reserved.
//

#ifndef GLPlayground_utilitaire_h
#define GLPlayground_utilitaire_h

float get_time(){
	static chrono::system_clock::time_point init_time = chrono::system_clock::now();
	chrono::system_clock::time_point now_time = chrono::system_clock::now();
	return chrono::duration_cast<chrono::milliseconds>(now_time - init_time).count() / 1000.f;
}

// tres utile!
// owi
// sisi

/* report GL errors, if any, to stderr */
void checkError(const char *functionName)
{
    GLenum error;
    while (( error = glGetError() ) != GL_NO_ERROR)
    {
        fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
    }
}

void dumpInfo(void)
{
    printf ("Vendor: %s\n", glGetString (GL_VENDOR));
    printf ("Renderer: %s\n", glGetString (GL_RENDERER));
    printf ("Version: %s\n", glGetString (GL_VERSION));
    printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
    checkError ("dumpInfo");
}

// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void printShaderInfoLog(GLint shader)
{
	GLint infoLogLen = 0;
	GLsizei charsWritten = 0;
	GLchar *infoLog;
    
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
    
	// should additionally check for OpenGL errors here
    
	if (infoLogLen > 0)
	{
		infoLog = (GLchar *)malloc(infoLogLen); //new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		printf("Infolog: %s\n", infoLog);
		free(infoLog);
	}
	else
		printf("No infolog\n");
    
	// should additionally check for OpenGL errors here
}

#endif
