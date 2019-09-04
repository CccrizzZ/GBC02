
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <time.h>
#include <array>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint Buffers[NumBuffers];


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

	//Generating two buffers, one is used to store the coordinates of the vertices
	//The other one is not used. Just wanted to show that we can allocate as many as buffers, some of which might left unused.
	glGenBuffers(2, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Selecting the buffer
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	//Randomizing vertices coordinates
	for (int i = 0; i < NumVertices; i++)
	{
		float x = (rand() % 40 + 1) / 100.0f - 1;
		float y = (rand() % 40 + 1) / 100.0f - 1;
		vertices[i][0] = x;
		vertices[i][1] = y;

	}

	//Pushing the coordinates of the vertices into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_LINE_STRIP, 0, NumVertices);

	glFlush();
}

void idle()
{
	glutPostRedisplay();
}

glm::mat3 Mat4Trim(glm::mat4 input, int r, int c) {
	glm::mat4 inputs;
	glm::mat3 result;

	int temp[16];

	for (int i = 0; i < input.length(); i++)
	{
		for (int j = 0; j < input.length(); j++)
		{
			
		}
	}

	for (int i = 0; i < result.length() ; i++)
	{
		for (int j = 0; j < result.length(); j++)
		{
			cout << result[i][j];
		}
		cout << "\n";
	}

	return result;

}



//---------------------------------------------------------------------
//
// main
//

int
main(int argc, char** argv)
{

	glm::mat4 testmat = glm::mat4(1.0);

	srand(time(NULL));

	for (int i = 0; i < testmat.length(); i++)
	{
		for (int j = 0; j < testmat.length(); j++)
		{
			int temp = rand() % 10;
			testmat[i][j] = temp;
		}
	}


	for (int i = 0; i < testmat.length(); i++)
	{
		for (int j = 0; j < testmat.length(); j++)
		{
			cout << testmat[i][j];
		}
		cout << '\n';
	}
	
	int delRow;
	int delCol;

	cout << "deleting row:" ;
	cin >> delRow;

	cout << "deleting column:";
	cin >> delCol;

	Mat4Trim(testmat, delRow, delCol);





	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("First GL Project");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}
