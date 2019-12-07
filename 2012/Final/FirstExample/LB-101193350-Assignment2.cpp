
//***************************************************************************
// HG-22155-Assignment2.cpp by Galal Hassan (C) 2019 All Rights Reserved.
//
// Assignment 2 submission.
//
// Description:
//   Click run to see the results.
//***************************************************************************
using namespace std;
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Box.h"
#include <vector>

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)
#define XY_AXIS glm::vec3(1, 1, 0)
#define YZ_AXIS glm::vec3(0, 1, 1)
#define ZX_AXIS glm::vec3(1, 0, 1)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };


// Horizontal and vertical ortho offsets.
float posX, posY, posZ = 2.0f, scrollSpd = 0.25f;
float rotang = 0;

glm::mat4 mvp;
glm::mat4 view;
glm::mat4 projection;

GLuint gVAO;		// Vertex Array Object
GLuint modelID;

vector<Box*> VBox;

Box *b1 = new Box(1.0f, rotang, glm::vec3(1.0f), Z_AXIS, glm::vec3(0.0f, 0.0f, -10.0f));



void init(void)
{
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up


	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	modelID = glGetUniformLocation(program, "mvp");

	// in world coordinates
	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); 

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(posX, posY, posZ),		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);


	// generate and bind Vertex Array Object
	gVAO = 0;
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	
	
	// init the box
	b1->initBox();


	// Enable depth test
	glEnable(GL_DEPTH_TEST);

}



//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	view = glm::lookAt(
		glm::vec3(posX, posY, posZ),		// Camera pos in World Space
		glm::vec3(posX, posY, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Background Color
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);

	// Set Camera Projection
	projection = glm::perspective(30.0f, (GLfloat)1.0f, 1.0f, 50.0f);

	glBindVertexArray(gVAO);
	

	// display the box
	b1->displayBox(modelID,mvp, view, projection);
	rotang += 1.0f;

	glutSwapBuffers(); // Instead of double buffering.
}

// Key down
void keyDown(unsigned char key, int x, int y)
{
	// Orthographic.
	switch(key)
	{
		case 'w':
			posZ -= 0.1f;
		break;
		case 's':
			posZ += 0.1f;
		break;
		case 'a':
			posX += 0.1f;
		break;
		case 'd':
			posX -= 0.1f;
		break;
		case 'r':
			posY -= 0.1f;
		break;
		case 'f':
			posY += 0.1f;
		break;
	}

}



void idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(768, 768);
	glutCreateWindow("Liu Beining 101193350");
	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();




	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyDown);


	glutMainLoop();



}
