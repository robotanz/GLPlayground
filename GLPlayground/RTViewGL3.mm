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
#include "ShaderProgram.h"

/* VARIABLES GLOBALES */

int win_w = 800;
int win_h = 600;
vec2 mouse;
vector<vec4> light_table;
mat4 view_matrix, proj_matrix, model_matrix;
int static const NB_VBO = 2;
int static const NB_VAO = 3;
GLuint vertexArrayObjID[NB_VAO]; // VAO
#include "Torus.h"
#include "Ground.h"
#include "Cube.h"
vector<Mesh*> geometries;
ShaderProgram *program;
string vsShader = "vertexShader_old.glsl";
string fsShader = "fragmentShader.glsl";


void init(void)
{
	// changement de la couleur de fond
    glClearColor(0.0f,0.0f,0.0f,1.0f);
   	glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);
	checkError("GL inits");
    
    glGenVertexArrays(NB_VAO, vertexArrayObjID);
    
    /* FILL GEOMETRY */
    
    geometries.push_back(new Torus(0.2f, 50, 50));
    geometries.push_back(new Cube());
    geometries.push_back(new Ground());
    
    for(int i = 0 ; i < geometries.size() ; ++i){
        geometries[i]->make();
        geometries[i]->upload();
    }
    
    /* SHADERS & PROGRAM */
    
    program = new ShaderProgram(vsShader, fsShader);
    
    /* CAMERA */
    
	view_matrix = lookAt(vec3(0.f, 1.8f, 3.f), vec3(0.f, 1.5f, 0.f), vec3(0.f, 1.f, 0.f));
	proj_matrix = perspective(50.f, (float) win_w / win_h, 0.2f,100.0f);
    
	/* LIGHTS */
    
    light_table.push_back(vec4(1.f, 1.8f, -3.f, 1.f));
	light_table.push_back(vec4(0.8f, 0.5f, 0.5f, 50.f)); // color
	light_table.push_back(vec4(-0.2f, -1.0f, 0.f, 0.f));
	light_table.push_back(vec4(0.f, 1.f, 1.0f, 10.f)); // color
    
	checkError("init");
}

void display(void)
{
	checkError("pre display");
    
    glViewport(0, 0, win_w, win_h);
    
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float time = get_time();
    
    program->useProgram();
	program->sendUniforms(time, mouse, view_matrix, proj_matrix, light_table);
    
    for(int i = 0 ; i < geometries.size() ; ++i){
        geometries[i]->updateMatrixModel(model_matrix, time, mouse, program->getUniform(program->UniformModelMatrix));
        geometries[i]->setSpecialParameters();
        geometries[i]->bind();
        geometries[i]->draw(program);
        geometries[i]->unbind();
    }
    
    program->stopProgram();
    
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

    // Bundle resources path with appended /, used to read GLSL shaders from Resources
    NSString *resourcePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"];
    const char *str = [resourcePath cStringUsingEncoding:NSASCIIStringEncoding];
    //printf("Bundle Path: %s\n", str);
    
    // set as base path for shaders (making a C++ string)
    ShaderProgram::setBaseDirectory(string(str));
    
	NSRect frame = NSMakeRect(100., 100., win_w, win_h);
    
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