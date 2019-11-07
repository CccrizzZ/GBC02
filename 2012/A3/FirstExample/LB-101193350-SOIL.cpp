
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
#define XYZ_AXIS glm::vec3(1, 1, 1)


enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };


// Horizontal and vertical ortho offsets.
float posX, posY, posZ = 6.0f, scrollSpd = 0.25f;
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
GLuint cube_tex;

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
		// FRONT
		0.25f, 0.333f, // 0
		0.5f, 0.333f, // 1
		0.5f, 0.666f, // 2
		0.25, 0.666f, // 3

		// BACK
		0.75f, 0.333f, // 0
		1.0f, 0.333f, // 1
		1.0f, 0.666f, // 2
		0.75, 0.666f, // 3

		// TOP
		0.25f, 0.0f, // 0
		0.5f, 0.0f, // 1
		0.5f, 0.333f, // 2
		0.25, 0.333f, // 3

		// BOT
		0.25f, 0.666f, // 0
		0.5f, 0.666f, // 1
		0.5f, 1.0f, // 2
		0.25, 1.0f, // 3

		// RIGHT
		0.0f, 0.333f, // 0
		0.25f, 0.333f, // 1
		0.25f, 0.666f, // 2
		0.0, 0.666f, // 3

		// LEFT
		0.5f, 0.333f, // 0
		0.75f, 0.333f, // 1
		0.75f, 0.666f, // 2
		0.5, 0.666f, // 3


	};

	GLushort cube_indices[] = {
		// front
		0,1,2,3,

		// back
		4,5,6,7,

		// top
		8,9,10,11,

		// bot
		12,13,14,15,

		// left
		16,17,18,19,

		// right
		20,21,22,23
	};

	// Cube verticies
	GLfloat cube_vertices[] = {
		// FRONT
		-0.65f, -0.65f, 0.65f,		// 0.
		0.65f, -0.65f, 0.65f,		// 1.
		0.65f, 0.65f, 0.65f,		// 2.
		-0.65f, 0.65f, 0.65f,		// 3.

		//BACK
		-0.65f, -0.65f, -0.65f,		// 4.
		0.65f, -0.65f, -0.65f,		// 5.
		0.65f, 0.65f, -0.65f,		// 6.
		-0.65f, 0.65f, -0.65f,		// 7.

		// TOP
		-0.65f, -0.65f, 0.65f,		// 8.
		-0.65f, -0.65f, -0.65f,		// 9.
		0.65f, -0.65f, -0.65f,		// 10.
		0.65f, -0.65f, 0.65f,		// 11.

		//BOTTOM
		-0.65f, 0.65f, 0.65f,		// 12.
		-0.65f, 0.65f, -0.65f,		// 13.
		0.65f, 0.65f, -0.65f,		// 14.
		0.65f, 0.65f, 0.65f,		// 15.

		// LEFT
		-0.65f, -0.65f, -0.65f,		// 16.
		-0.65f, -0.65f, 0.65f,		// 17.
		-0.65f, 0.65f, 0.65f,		// 18.
		-0.65f, 0.65f, -0.65f,   	// 19.

		//RIGHT
		0.65f, -0.65f, 0.65f,		// 20.
		0.65f, -0.65f, -0.65f,		// 21.
		0.65f, 0.65f, -0.65f,		// 22.
		0.65f, 0.65f, 0.65f,		// 23.
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
	cube_tex = 0;
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SOIL_free_image_data(image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

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
		glm::vec3(posX, posY, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Background Color
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);

	// Set Camera Projection
	projection = glm::perspective(30.0f, (GLfloat)1.0f, 1.0f, 50.0f);

	// First Cube
	transformObject(1.0f, XYZ_AXIS, rotAngle-=2, glm::vec3(-3.0f, 0.0f, 0.0f));
	glBindVertexArray(gVAO);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	// Ordering GPU to start the pipeline
	glDrawElements(GL_QUADS, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// Second Cube
	transformObject(1.0f, XYZ_AXIS, rotAngle1+=2, glm::vec3(3.0f, 0.0f, 0.0f));
	glBindVertexArray(gVAO);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	// Ordering GPU to start the pipeline
	glDrawElements(GL_QUADS, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);



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

void clean(){
	std::cout << "Cleaning Up" << std::endl;
	glDeleteTextures(1, &cube_tex);
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
