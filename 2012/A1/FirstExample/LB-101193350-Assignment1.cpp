
//***************************************************************************
// HG-22155-Assignment1.cpp by Galal Hassan (C) 2019 All Rights Reserved.
//
// Assignment 1 submission.
//
// Description:
//   Click run to see the results.
//***************************************************************************
using namespace std;

#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };




GLuint Buffers[NumBuffers];
GLuint gVAO;
GLuint MatrixID;
glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Projection;



const GLuint NumVertices = 10;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2];

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

	// Get a handle for "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");

	// Projection matrix in world coordinate
	Projection = glm::ortho(
		-10.0f, 10.0f, -10.0f,
		10.0f, 0.0f, 100.0f
	);

	// Camera matrix
	View = glm::lookAt(
		glm::vec3(0, 0, 1.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);







	// Graph a square
	GLfloat points[] = {
		-0.5f, 0.5f ,0.0f,
		0.5f, 0.5f ,0.0f,
		0.5f, -0.5f ,0.0f,
		-0.5f, -0.5f ,0.0f
	};

	GLuint points_vbo = 0;
	glGenBuffers(0, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE ,0 ,0);
	glEnableVertexAttribArray(0);


	// Graph color
	GLfloat colors[] = {
		-0.5f, 0.5f ,0.0f,
		0.5f, 0.5f ,0.0f,
		0.5f, -0.5f ,0.0f,
		-0.5f, -0.5f ,0.0f
	};

	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE ,0 ,0);
	glEnableVertexAttribArray(1);

}




// Transform
void transformObject(float scale, glm::vec3 rotationAxis ,float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}




//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Selecting the buffer
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);

	// Background Color
	glClearColor(0.0f, 2.0f, 0.6f, 0.0f);

	// glBufferData(GL_ARRAY_BUFFER, sizeof)
	// transformObject(12.0f, Z_AXIS, 0, glm::vec3(0, 0, 0));
	glDrawArrays(GL_LINE_LOOP, 0, 8);




	glFlush();
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
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(768, 768);
	glutCreateWindow("Liu Beining 101193350");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}
