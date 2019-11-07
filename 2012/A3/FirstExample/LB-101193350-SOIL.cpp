
//***************************************************************************
// HG-22155-SOIL.cpp by Galal Hassan (C) 2019 All Rights Reserved.
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
#include <SoilLib/SOIL.h>

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
float posX, posY, posZ = 12.0f, scrollSpd = 0.25f;
float rotAngle = 0.0f;
float rotAngle1 = 0.0f;



glm::mat4 mvp;
glm::mat4 view;
glm::mat4 projection;

GLuint gVAO;		// Vertex Array Object
GLuint ibo;			// Index Buffer Object
GLuint points_vbo;	// Vertex Buffer Object (Points)
GLuint colors_vbo;	// Vertex Buffer Object (Colors)
GLuint modelID;
GLuint cube_tex_vbo;		// Vertex Array Object
GLuint cube_tex_ibo;			// Index Buffer Object
GLuint cube_tex_ibo2;
GLuint cube_tex_vbo2;


// Initiallization Function
void init(void)
{
	// cout << "Enter cube amount:";
	// cin >> numCube;



	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up




	modelID = glGetUniformLocation(program, "mvp");

	// in world coordinates
	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); 

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(posX, posY, posZ),		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);


	// SOIL textures
	GLfloat textureCoordinates[] ={

		// 0.0f, 0.55f, // 0
		// 0.55f, 0.55f, // 1
		// 0.55f, 0.0f,	// 2
		// 0.0f, 0.0f, // 3

		0.25f, 0.333f, // 0
		0.5f, 0.333f, // 1
		0.5f, 0.666f, // 2
		0.25, 0.666f, // 3


		0.25f, 0.333f, // 0
		0.5f, 0.333f, // 1
		0.5f, 0.666f, // 2
		0.25, 0.666f, // 3




	};

	GLushort cube_index_array[] = {
		// front
		0,1,2,3,
		// back
		4,5,6,7,

		// // top
		// 4,5,1,0,
		// // bot
		// 2,3,7,6,

		// // left
		// 2,6,5,1,
		// // right
		// 0,4,7,3
	};







	// Cube indicies
	GLshort cube_indices[] = {
		// Front.
		3, 2, 1, 0, 
		// Left.
		0, 3, 7, 4,
		// Bottom.
		4, 0, 1, 5,
		// Right.
		5, 1, 2, 6,
		// Back.
		6, 5, 4, 7,
		// Top.
		7, 6, 2, 3
	};

	// Cube verticies
	GLfloat cube_vertices[] = {
		-0.9f, -0.9f, 0.9f,		// 0.
		0.9f, -0.9f, 0.9f,		// 1.
		0.9f, 0.9f, 0.9f,		// 2.
		-0.9f, 0.9f, 0.9f,		// 3.
		-0.9f, -0.9f, -0.9f,	// 4.
		0.9f, -0.9f, -0.9f,		// 5.
		0.9f, 0.9f, -0.9f,		// 6.
		-0.9f, 0.9f, -0.9f,		// 7.
	};

	// Cube colors
	GLfloat cube_colors[] = { 
		1.0f, 0.0f, 0.0f,		
		1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	// generate and bind Vertex Array Object
	gVAO = 0;
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	
	// generate and bind Index Buffer Object
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// generate and bind points Vertex Buffer Object(VBO)
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	
	// generate and bind colors Vertex Buffer Object(VBO)
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);


	// Load image using SOIL
	GLint width, height;
	unsigned char* image = SOIL_load_image("rubiksCube.png", &width, &height, 0 , SOIL_LOAD_RGB);
	if (!image)
	{
		printf("ERROR: image not found! \n");
	}
	else
	{
		printf("Image loaded! \n");
	}
	
	// Enable and bind texture
	glActiveTexture(GL_TEXTURE0);
	GLuint cube_tex = 0;
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	// generate and bind Index Buffer Object
	cube_tex_ibo = 0;
	glGenBuffers(1, &cube_tex_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_tex_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_array), cube_index_array, GL_STATIC_DRAW);

	// Create vbo
	cube_tex_vbo = 0;
	glGenBuffers(1, &cube_tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);



	// Enable depth test
	glEnable(GL_DEPTH_TEST);

}


// Object Transformation Function 1
void transformObject2(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation){
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);
	mvp = projection * view * Model;
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
}

// Object Transformation Function 2
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	mvp = projection * view * Model;
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
}



// Display Function
void
display(void)
{
	view = glm::lookAt(
		glm::vec3(posX, posY, posZ),		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Background Color
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);

	// Set Camera Projection
	projection = glm::perspective(30.0f, (GLfloat)1.0f, 1.0f, 50.0f);



	// // Ground
	// glBindVertexArray(gVAO);
	// // Set cube transforamtion
	// transformObject2(glm::vec3(100.0f, 1.0f, 100.0f), YZ_AXIS, 1.0f, glm::vec3(0.0f, 2.0f, 0.0f));
	// // Draw cube
	// glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);

	for (int i = 0; i <= 2; i++)
	{

		if (i==0)
		{
			glBindVertexArray(gVAO);
			transformObject2(glm::vec3(1.0f, 1.0f, 1.0f), XY_AXIS, 1.0f, glm::vec3(3.0f, 0.0f, 0.0f));
			glDrawElements(GL_QUADS, 64, GL_UNSIGNED_SHORT, 0);	
		}
		else if (i==1)
		{
			glBindVertexArray(gVAO);
			transformObject2(glm::vec3(1.0f, 1.0f, 1.0f), X_AXIS, 90.0f, glm::vec3(3.0f, 0.0f, 0.0f));
			glDrawElements(GL_QUADS, 64, GL_UNSIGNED_SHORT, 0);	
		}
		else if (i==2)
		{
			glBindVertexArray(gVAO);
			transformObject2(glm::vec3(1.0f, 1.0f, 1.0f), Y_AXIS, 90.0f, glm::vec3(3.0f, 0.0f, 0.0f));
			glDrawElements(GL_QUADS, 64, GL_UNSIGNED_SHORT, 0);	
		}
		
	}
	
	for (int i = 0; i <= 2; i++)
	{

		if (i==0)
		{
			glBindVertexArray(gVAO);
			transformObject2(glm::vec3(1.0f, 1.0f, 1.0f), XY_AXIS, 1.0f, glm::vec3(-3.0f, 0.0f, 0.0f));
			glDrawElements(GL_QUADS, 64, GL_UNSIGNED_SHORT, 0);	
		}
		else if (i==1)
		{
			glBindVertexArray(gVAO);
			transformObject2(glm::vec3(1.0f, 1.0f, 1.0f), X_AXIS, 90.0f, glm::vec3(-3.0f, 0.0f, 0.0f));
			glDrawElements(GL_QUADS, 64, GL_UNSIGNED_SHORT, 0);	
		}
		else if (i==2)
		{
			glBindVertexArray(gVAO);
			transformObject2(glm::vec3(1.0f, 1.0f, 1.0f), Y_AXIS, 90.0f, glm::vec3(-3.0f, 0.0f, 0.0f));
			glDrawElements(GL_QUADS, 64, GL_UNSIGNED_SHORT, 0);	
		}
		
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
