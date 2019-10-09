


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
#include "glm\gtc\type_ptr.hpp"
#include <iostream>
#include <typeinfo>
using namespace std;


#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)

int numSquare;
int rotAngle;

GLuint vao, points_vbo, colours_vbo, modelID;

//const GLfloat scale = 0.5f;

GLfloat points[] = {
	-0.9f,  0.9f,  0.0f,
	0.9f,  0.9f,  0.0f,
	0.9f, -0.9f,  0.0f,
	-0.9f, -0.9f,  0.0f
};

GLfloat colours[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};


void Qsquare() {
	cout << "Enter Amount of Square: ";
	cin >> numSquare;
	cout << "Enter Amount of Rotation: ";
	cin >> rotAngle;
}


float r[128];
float g[128];
float b[128];

void init(void)
{
	for (int i = 0; i < 128; i++)
	{
		// colors
		r[i] = float(rand() % 255) / 100.0f;
		g[i] = float(rand() % 255) / 100.0f;
		b[i] = float(rand() % 255) / 100.0f;

	}

	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	modelID = glGetUniformLocation(program, "model");

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.
}



// Transform
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
}



//---------------------------------------------------------------------
//
// display
//


void
display(void)
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT);
	// Ordering the GPU to start the pipline
	glBindVertexArray(vao);

	// Background Color
	glClearColor(0.0f, 0.2f, 0.1f, 1.0f);

	bool flag = true;
	float s = 1.0f;
	int cocount = 0;
	for (int i = 1; i < numSquare + 1; i++, s /= 2)
	{


		// change s
		s = sqrt(s * s + s * s);



		float colors[] = {
			r[cocount], g[cocount], b[cocount],
			r[cocount], g[cocount], b[cocount],
			r[cocount], g[cocount], b[cocount],
			r[cocount], g[cocount], b[cocount],
		};
		cocount++;
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


		// transformation
		transformObject(s / 1.5, Z_AXIS, flag == false ? float(rotAngle) : 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));


		// flags
		if (i % 2 - 1 == 0)
		{
			flag = false;
		}
		else
		{
			flag = true;
		}

		// Ordering the GPU to start the pipeline
		glDrawArrays(GL_LINE_LOOP, 0, 4);

	}
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
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Liu Beining 101193350");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	Qsquare();
	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}






