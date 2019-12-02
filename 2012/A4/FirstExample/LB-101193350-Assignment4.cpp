
//***************************************************************************
// HG-22155-Assignment3.cpp by Galal Hassan (C) 2018 All Rights Reserved.
// Assignment 3 submission. 
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
#include "glm\gtc\type_ptr.hpp"
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



GLuint gVAO;		// Vertex Array Object
GLuint ibo;			// Index Buffer Object
GLuint points_vbo;	// Vertex Buffer Object (Points)
GLuint colors_vbo;	// Vertex Buffer Object (Colors)
GLuint cube_tex_vbo;		// Vertex Buffer Object
GLuint cube_tex;			// Texture


GLuint MVPID;
GLuint modelID;
GLuint projID;
GLuint viewID;
GLuint diffuseTextID;

glm::mat4 mvp;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 currentLightPos;

glm::vec3 currentCamPos;
glm::vec3 currentCamVel;

int frame=0, currentTime, timebase=0;
float deltaTime = 0;
bool keyStates[256] = {};

struct PointLight
{
	GLuint posHandle;
	GLuint colorHandle;
	GLuint strengthHandle;
};

struct DirectLight
{
	GLuint dirHandle;
	GLuint colorHandle;
	GLuint strengthHandle;
};

struct Light
{
	GLuint colorHandle;
	GLuint posHandle;
	GLuint strengthHandle;
	GLuint falloffStartHandle;
	GLuint falloffEndHandle;
};

Light pointLights[2];




// Initiallization Function
void init(void)
{

	// Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	// Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	// SOIL textures
	GLfloat textureCoordinates[] = {
		// FRONT
		0.25f, 0.333f, // 0
		0.5f, 0.333f,  // 1
		0.5f, 0.666f,  // 2
		0.25, 0.666f,  // 3

		// // BACK
		// 0.75f, 0.333f, // 0
		// 1.0f, 0.333f,  // 1
		// 1.0f, 0.666f,  // 2
		// 0.75, 0.666f,  // 3

		// // TOP
		// 0.25f, 0.0f,  // 0
		// 0.5f, 0.0f,   // 1
		// 0.5f, 0.333f, // 2
		// 0.25, 0.333f, // 3

		// // BOT
		// 0.25f, 0.666f, // 0
		// 0.5f, 0.666f,  // 1
		// 0.5f, 1.0f,	// 2
		// 0.25, 1.0f,	// 3

		// // RIGHT
		// 0.0f, 0.333f,  // 0
		// 0.25f, 0.333f, // 1
		// 0.25f, 0.666f, // 2
		// 0.0, 0.666f,   // 3

		// // LEFT
		// 0.5f, 0.333f,  // 0
		// 0.75f, 0.333f, // 1
		// 0.75f, 0.666f, // 2
		// 0.5, 0.666f,   // 3

	};

	GLshort cube_indices[] = {
		0, 1, 2, 3,
		4, 5, 6,
		// 7,
		// 8,9,10,11,
		// 12,13,14,15,
		// 16,17,18,19,
		// 20,21,22,23
	};

	// Cube verticies
	GLfloat cube_vertices[] = {
		//  X     Y     Z       U     V          Normal
		// bottom
		-1.0f,
		-1.0f,
		-1.0f,
		0.0f,
		0.0f,
		0.0f,
		-1.0f,
		0.0f,
		1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		0.0f,
		0.0f,
		-1.0f,
		0.0f,
		-1.0f,
		-1.0f,
		1.0f,
		0.0f,
		1.0f,
		0.0f,
		-1.0f,
		0.0f,
		1.0f,
		-1.0f,
		-1.0f,
		1.0f,
		0.0f,
		0.0f,
		-1.0f,
		0.0f,
		1.0f,
		-1.0f,
		1.0f,
		1.0f,
		1.0f,
		0.0f,
		-1.0f,
		0.0f,
		-1.0f,
		-1.0f,
		1.0f,
		0.0f,
		1.0f,
		0.0f,
		-1.0f,
		0.0f,
	};

	// Init Lights
	// 1. get position
	pointLights[0].colorHandle = glGetUniformLocation(program, "pointLights[0].color");
	pointLights[0].posHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[0].strengthHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[0].falloffStartHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[0].falloffEndHandle = glGetUniformLocation(program, "pointLights[0].Position");

	pointLights[1].colorHandle = glGetUniformLocation(program, "pointLights[0].color");
	pointLights[1].posHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[1].strengthHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[1].falloffStartHandle = glGetUniformLocation(program, "pointLights[0].Position");
	pointLights[1].falloffEndHandle = glGetUniformLocation(program, "pointLights[0].Position");

	// 2. pass data
	glUniform3fv(pointLights[0].colorHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
	currentLightPos = glm::vec3(2.0f, 2.0f, 1.0f);
	glUniform3fv(pointLights[0].posHandle, 1, glm::value_ptr(currentLightPos));
	glUniform1f(pointLights[0].strengthHandle, 1.0f);
	glUniform1f(pointLights[0].falloffStartHandle, 1.0f);
	glUniform1f(pointLights[0].falloffEndHandle, 50.0f);

	glUniform3fv(pointLights[1].colorHandle, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
	glUniform3fv(pointLights[1].posHandle, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glUniform1f(pointLights[1].strengthHandle, 1.0f);
	glUniform1f(pointLights[1].falloffStartHandle, 1.0f);
	glUniform1f(pointLights[1].falloffEndHandle, 50.0f);


	// Init diffuse texture
	diffuseTextID = glGetUniformLocation(program, "diffuseTexture");

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

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	// Load image using SOIL
	GLint width, height;
	glActiveTexture(GL_TEXTURE0);



	unsigned char *image = SOIL_load_image("bonusTexture.png", &width, &height, 0, SOIL_LOAD_RGBA);


	// Enable and bind texture
	cube_tex = 0;
	glGenTextures(1, &cube_tex);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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

	// Get Location
	modelID = glGetUniformLocation(program, "mvp");
	projID = glGetUniformLocation(program, "projection");
	viewID = glGetUniformLocation(program, "view");

	// in world coordinates
	projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); 

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(posX, posY, posZ),		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);

	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniform3fv(pointLights[0].posHandle, 1, glm::value_ptr(currentLightPos));






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
	glUniformMatrix4fv(projID, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
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
	glUniformMatrix4fv(projID, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}



// Display Function
void
display(void)
{
	view = glm::lookAt(
		glm::vec3(posX, posY, posZ),	// Camera pos in World Space
		glm::vec3(posX, posY, 0),		// Looks at the origin
		glm::vec3(1, 0, 0)	            //  X axis is X
	);

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Background Color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// // Set Camera Projection
	projection = glm::perspective(30.0f, (GLfloat)1.0f, 1.0f, 50.0f);

	

	// First Cube
	transformObject(1.0f, X_AXIS, rotAngle, glm::vec3(0.0f, 0.0f, 0.0f));
	
	glBindVertexArray(gVAO);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	// Ordering GPU to start the pipeline
	glDrawElements(GL_QUADS, 600, GL_UNSIGNED_SHORT, 0);
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
	glutInitWindowPosition(0,0);

	glutCreateWindow("Liu Beining 101193350");
	glewExperimental = true;
	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutIdleFunc(idle);
	init();






	glutMainLoop();



}
