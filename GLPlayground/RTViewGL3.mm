// Based on the Cocoa variant of the "minimal animation" demo
// Working, reasonably simple 3.2 demo!!!
// Preliminary demo by Ingemar Ragnemalm 2012
// for the 3.2 update of my course book.
// Partially based on other GL3.2 demos, especially that demo with two triangles.
// (Should be referenced I guess but I can't do that right now.)
// Later versions appears in my book.

// Fixed by Thomas Jgenti 2013

#import <Cocoa/Cocoa.h>
#include <OpenGL/gl3.h>

#include <cmath>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stddef.h>
#include <string.h>
using namespace std;

#define GLM_SWIZZLE 1
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/swizzle.hpp"
using namespace glm;


#include "utilitaire.h"

/* VARIABLES GLOBALES */

int win_w = 400;
int win_h = 400;
vec2 mouse;
vector<vec4> light_table;
mat4 view_matrix, proj_matrix;
int static const NB_VBO = 2;
int static const NB_VAO = 1;
GLuint vertexArrayObjID[NB_VAO]; // VAO
#include "mesh.h"
#include "shader.h"
mesh *torus;
program programs[PROGRAM_MAX]; // le program qui va les lier



void init(void)
{
	// changement de la couleur de fond
    glClearColor(0.0f,0.0f,0.0f,1.0f);
   	glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);
	checkError("GL inits");
    
    
    glGenVertexArrays(NB_VAO, vertexArrayObjID);
    
    /* TORUS MESH */
    
	torus = new mesh(0); // je donne en param l'indice du VAO correspondant
	//torus->make_quad();
    torus->make_torus(0.2f, 50, 50);
	torus->upload();
    
    /* SHADERS & PROGRAM */
    
	programs[ProgramLight].name = create_program("vertexShader_old.glsl", "fragmentShader.glsl");
	programs[ProgramLight].get_uniforms();
    
    /* CAMERA */
    
	view_matrix = lookAt(vec3(0.f, 1.8f, 3.f), vec3(0.f, 1.5f, 0.f), vec3(0.f, 1.f, 0.f)); // param1 : coordonnees de l'oeil. param2 : le centre de ce que l'on veut regarder (gere donc l'inclinaison). param3 : la vertical (un vecteur).
	proj_matrix = perspective(50.f, (float) win_w / win_h, 0.2f,100.0f); // param1 : en degres, angle de la camera. param2 : ratio. param3 : near. param4 : far.
    
	/* LIGHTS */
    
    light_table.push_back(vec4(1.f, 1.8f, -3.f, 1.f));// On place cette light 1 metre a droite de la camera + devant nous
	light_table.push_back(vec4(0.8f, 0.5f, 0.5f, 50.f)); // la couleur
	light_table.push_back(vec4(-0.2f, -1.0f, 0.f, 0.f)); // On place cette light au dessus du tore (directionnelle) un peu a gauche
	light_table.push_back(vec4(0.f, 1.f, 1.0f, 10.f)); // la couleur
    
	checkError("init");
}

void display(void)
{
	checkError("pre display");
    
    glViewport(0, 0, 1000, 800);
    
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // on va rebinder les buffers pour pouvoir les utiliser. Et le program
    glUseProgram(programs[ProgramLight].name);
	programs[ProgramLight].set_uniforms();
    
    /* RENDER TORUS */
    
	mat4 model_matrix;
	model_matrix = translate(mat4(1.f), vec3(0.f, 1.f + 1.f, 0.f)); // TRANSLATE => on monte le tore de 100cm en hauteur
	model_matrix = scale(model_matrix, vec3(0.30f));
	model_matrix = rotate(model_matrix, get_time() * 100.f, vec3(1.f, 1.f, 0.f));
	// SCALE => param1 : transformation precedente, ici on met matrice unite (le 1.f est juste en diagonale). param2 : le 0.30f par contre se met dans chaque composoante. Le tore fera 30cm de diametre.
	// ROTATE => param2 : angle. param3 : axe de rotation.
    
	glUniformMatrix4fv(programs[ProgramLight].uniforms[UniformModelMatrix], 1, GL_FALSE, &model_matrix[0][0]);
	torus->bind();
    torus->draw(programs[ProgramLight].name);
	torus->unbind();
    
    glUseProgram(0);
    
	checkError("display");
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

// -----------

// Globals (was in GLViewDataPtr)
NSOpenGLContext	*m_context;
float lastWidth, lastHeight;
NSView *theView;

void MakeContext(NSView *view)
{
    //	NSWindow *w;
	NSOpenGLPixelFormat *fmt;
    
	NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 32,
		0
	};
    
	// Create custom data pointer
	theView = view;
    
	// Init GL context
	fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs];
    
	m_context = [[NSOpenGLContext alloc] initWithFormat: fmt shareContext: nil];
	[fmt release];
	[m_context makeCurrentContext];
    
	checkError("makeCurrentContext");
}



// -------------------- View ------------------------

@interface TestView : NSView { }
-(void)drawRect:(NSRect)rect;
@end

float loop;

#define Pi 3.1415

@implementation TestView

-(void)drawRect:(NSRect)rect
{
	if (([theView frame].size.width != lastWidth) || ([theView frame].size.height != lastHeight))
	{
		lastWidth = [theView frame].size.width;
		lastHeight = [theView frame].size.height;
        
		// Only needed on resize:
		[m_context clearDrawable];
        
		reshape([theView frame].size.width, [theView frame].size.height);
	}
    
	[m_context setView: theView];
	[m_context makeCurrentContext];
    
	// Draw
	display();
    
	[m_context flushBuffer];
	[NSOpenGLContext clearCurrentContext];
    
	loop = loop + 0.1;
}

-(void)windowWillClose:(NSNotification *)note
{
    [[NSApplication sharedApplication] terminate:self];
}
@end


// -------------------- Timer ------------------------

// Mini-mini class for the timer
@interface TimerController : NSObject { }
-(void)timerFireMethod:(NSTimer *)t;
@end

NSTimer	*gTimer;
TimerController	*myTimerController;
NSView	*view;

// Timer!
@implementation TimerController
-(void)timerFireMethod:(NSTimer *)t;
{
	[view setNeedsDisplay: YES];
}
@end

// ------------------ Main program ---------------------

NSApplication *myApp;
NSAutoreleasePool *pool;
NSWindow *window;

int main(int argc, const char *argv[])
{
	pool = [NSAutoreleasePool new];
	myApp = [NSApplication sharedApplication];
   // printf("Current Path: %s\n",[[[NSFileManager defaultManager] currentDirectoryPath] cString]);
    
	NSRect frame = NSMakeRect(100., 100., 1000., 800.);
    
	window = [NSWindow alloc];
	[window initWithContentRect:frame
                      styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
                        backing:NSBackingStoreBuffered
                          defer:false];
	[window setTitle:@"Cocoa OpenGL Playground"];
    
	view = [TestView alloc];
	[view initWithFrame: frame];
    
	// OpenGL init!
	MakeContext(view);
    
    init ();
    //	initShaders();  -> devient inutile
    dumpInfo ();
    
	[window setContentView: view];
	//[window setDelegate: view];
	[window makeKeyAndOrderFront: nil];
    
	// Timer
	myTimerController = [TimerController alloc];
	gTimer = [NSTimer
              scheduledTimerWithTimeInterval: 0.02
              target: myTimerController
              selector: @selector(timerFireMethod:)
              userInfo: nil
              repeats: YES];
    
	// Main loop
	[myApp run];
	[pool release]; // Free;
	return( EXIT_SUCCESS );
}