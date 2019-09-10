
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

glm::mat3 Mat4Trim(glm::mat4 &input, int r, int c) {
	// temporary 3x3 matrix
	glm::mat3 tempMtx;
	// temporary 2d list
	int temp[16];

	// fill the temp list with input 4x4 matrix except for row r and column c
	int counter = 0;
	for (int i = 0; i < input.length(); i++)
	{
		for (int j = 0; j < input.length(); j++)
		{
			if (j != c && i != r)
			{
				temp[counter] = input[i][j];
				counter++;
			}
		}
	}

	//  fill the result matrix with the temp list
	int counter2 = 0;
	for (int i = 0; i < tempMtx.length(); i++)
	{
		for (int j = 0; j < tempMtx.length(); j++)
		{
			tempMtx[i][j] = temp[counter2];
			counter2++;
		}
	}
	
	return tempMtx;
}



//---------------------------------------------------------------------
//
// main
//

int
main(int argc, char** argv)
{
	// init 4x4 matrix
	glm::mat4 testmat = glm::mat4(1.0);
	// Rand
	srand(time(NULL));
	// fill this matrix with random numbers
	for (int i = 0; i < testmat.length(); i++)
	{
		for (int j = 0; j < testmat.length(); j++)
		{
			int temp = rand() % 10;
			testmat[i][j] = temp;
		}
	}

	// print this 4x4 matrix
	for (int i = 0; i < testmat.length(); i++)
	{
		for (int j = 0; j < testmat.length(); j++)
		{
			cout << testmat[i][j];
		}
		cout << '\n';
	}
	

	// rows and column
	int delRow = 0;
	int delCol = 0;

	// input for row and column
	cout << "deleting row:" ;
	cin >> delRow;
	delRow--;

	cout << "deleting column:";
	cin >> delCol;
	delCol--;

	// init 3x3 matrix
	glm::mat3 resultMtx = Mat4Trim(testmat, delRow, delCol);

	// print result 3x3 matrix
	for (int i = 0; i < resultMtx.length(); i++)
	{
		for (int j = 0; j < resultMtx.length(); j++)
		{
			cout << resultMtx[i][j];
		}
		cout << "\n";
	}











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
