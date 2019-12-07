
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
float posX = 0.0f , posY = -25.0f, posZ = 10.0f , scrollSpd = 0.25f;
float rotang = 0;

glm::mat4 mvp;
glm::mat4 view;
glm::mat4 projection;

GLuint gVAO;		// Vertex Array Object
GLuint modelID;

// vector for all boxes
vector<Box*> Boxes;




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



	// generate and bind Vertex Array Object
	gVAO = 0;
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	



	// create boxes and push into vector // (len, ang, scale, rotaxis, transition)
	Box *b1 = new Box(1.0f, 0.0f, glm::vec3(1.0f, 1.0f, 10.0f), Z_AXIS, glm::vec3(25.0f, 0.0f, 0.0f));
	Boxes.push_back(b1);

	Box *b2 = new Box(1.0f, 0.0f, glm::vec3(1.0f, 1.0f, 10.0f), Z_AXIS, glm::vec3(25.0f, 25.0f, 0.0f));
	Boxes.push_back(b2);

	Box *b3 = new Box(1.0f, 0.0f, glm::vec3(1.0f, 1.0f, 10.0f), Z_AXIS, glm::vec3(0.0f, 25.0f, 0.0f));
	Boxes.push_back(b3);

	Box *b4 = new Box(1.0f, 0.0f, glm::vec3(1.0f, 1.0f, 10.0f), Z_AXIS, glm::vec3(0.0f, 0.0f, 0.0f));
	Boxes.push_back(b4);


	Box *roof = new Box(1.0f, 0.0f, glm::vec3(25.0f, 25.0f, 1.0f), Z_AXIS, glm::vec3(12.5f, 12.5f, 10.0f));
	Boxes.push_back(roof);


	Box *Ground = new Box(1.0f, rotang, glm::vec3(150.0f, 150.0f, 0.1f), Z_AXIS, glm::vec3(0.0f, 0.0f, -5.0f));
	Boxes.push_back(Ground);


	// init the box in the vector
	for (int i = 0; i < Boxes.size(); i++)
	{
		Boxes[i]->initBox();
	}
	


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
		glm::vec3(posX, posY, posZ), // Camera pos in World Space
		glm::vec3(posX, 0, 0),	// and looks at the origin
		glm::vec3(0, -1, 0)			 // Head is up
	);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Background Color
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);

	// Set Camera Projection
	projection = glm::perspective(30.0f, (GLfloat)1.0f, 1.0f, 150.0f);

	glBindVertexArray(gVAO);

	rotang++;

	// init the boxes in the vector
	for (int i = 0; i < Boxes.size(); i++)
	{
		Boxes[i]->displayBox(modelID,mvp, view, projection);
	}



	glutSwapBuffers(); // Instead of double buffering.
}

// Key down
void keyDown(unsigned char key, int x, int y)
{
	// Orthographic.
	switch(key)
	{
		case 'w':
			posY += 0.5f;
		break;
		case 's':
			posY -= 0.5f;
		break;
		case 'a':
			posX -= 0.5f;
		break;
		case 'd':
			posX += 0.5f;
		break;
		case 'r':
			posZ += 0.5f;
		break;
		case 'f':
			posZ -= 0.5f;
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
