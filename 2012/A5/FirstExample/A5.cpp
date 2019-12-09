
///////////////////////////////////////////////////////////////////////
// LB-101193350-Assignment5.
// A5.cpp
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "SOIL.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)
#define XYZ_AXIS glm::vec3(1,1,1)

using namespace std;

// IDs.
GLuint vao, ibo, points_vbo, texture_vbo, normals_vbo, colours_vbo, modelID, projID, viewID;
GLuint program; // Updated to take this out of init.

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f, zoom = 10.0f;

// Fixed timestep variables.
int deltaTime, currentTime, lastTime = 0;

// Matrices.
glm::mat4 view, projection;

// Camera and transform variables.
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function.
GLfloat pitch, yaw,
moveSpeed = 0.1f,
turnSpeed = 1.0f;
float rotAngle = 0.0f;

// Mouse variables.
bool mouseFirst = true, mouseClicked = false;
int lastX, lastY;

// Texture variables.
GLuint textureID, textureID2;
GLint width, height, bitDepth;

// Light variables.
struct Light
{
	glm::vec3 diffuseColour; 
	GLfloat diffuseStrength;
	Light(glm::vec3 dCol, GLfloat dStr)
	{
		
		diffuseColour = dCol;
		diffuseStrength = dStr;
	}
};

struct AmbientLight
{
	glm::vec3 ambientColour;
	GLfloat ambientStrength;
	AmbientLight(glm::vec3 aCol, GLfloat aStr)
	{
		ambientColour = aCol;
		ambientStrength = aStr;
	}
};


struct DirectionalLight : public Light
{
	glm::vec3 direction;
	DirectionalLight(glm::vec3 dir, glm::vec3 dCol, GLfloat dStr) 
		: Light(dCol, dStr)
	{
		direction = dir;
	}
};

struct PointLight : public Light
{
	glm::vec3 position; //= glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat constant, linear, exponent;
	PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp, 
		glm::vec3 dCol, GLfloat dStr) : Light(dCol, dStr)
	{
		position = pos;
		constant = con;
		linear = lin;
		exponent = exp;
	}
};

struct SpotLight : public PointLight
{
	glm::vec3 direction;
	GLfloat edge, edgeRad;
	SpotLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
		glm::vec3 dCol, GLfloat dStr, glm::vec3 dir, GLfloat e) : PointLight(pos, con, lin, exp, dCol, dStr)
	{
		direction = dir;
		edge = e;
		edgeRad = cosf(glm::radians(edge));
	}
};

struct Material
{
	GLfloat specularStrength;
	GLfloat shininess;
};

AmbientLight aLight(glm::vec3(1.0f, 1.0f, 1.0f),	// Ambient colour.
				    1.0f);							// Ambient strength.

DirectionalLight dLight(glm::vec3(1.0f, 1.0f, 0.0f), // Direction.
						glm::vec3(1.0f, 1.0f, 0.0f),  // Diffuse colour.
						0.0f);						  // Diffuse strength.

PointLight pLight(
	glm::vec3(1.0f, 1.0f, 1.0f), // Position.
	1.0f, 0.7f, 1.8f,			 // Constant, Linear, Exponent.
	glm::vec3(1.0f, 1.0f, 1.0f), // Diffuse colour.
	20.0f						 // Diffuse strength.
);

PointLight pLight2(
	glm::vec3(-1.0f, 1.0f, 1.0f), // Position.
	1.0f, 0.7f, 1.8f,			  // Constant, Linear, Exponent.
	glm::vec3(1.0f, 1.0f, 1.0f),  // Diffuse colour.
	20.0f						  // Diffuse strength.
);

SpotLight sLight(glm::vec3(0.0f, 1.0f, 1.0f),	// Position.
				 1.0f, 1.0f, 1.0f,				// Constant, Linear, Exponent.
				 glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
			 	 1.0f,							// Diffuse strength.
				 glm::vec3(0.0f, 0.0f, -1.0f),  // Direction.
				 45.0f);					    // Angle.

Material mat = { 1.0f, 32 }; // Alternate way to construct an object.

//---------------------------------------------------------------------
//
// resetView
//
void resetView()
{
	position = glm::vec3(0.0f, 5.0f, 15.0f);
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0, 1, 0);
	pitch = -15.0f;
	yaw = -90.0f;
}


//---------------------------------------------------------------------
//
// initSquare
//
void initSquare(void)
{
	int i;
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLushort indices[] = 
	{
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23
	};
	
	ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Cube verticies
	GLfloat vertices[72] =
		{
			// FRONT
			-0.65f, -0.65f, 0.65f, // 0.
			0.65f, -0.65f, 0.65f,  // 1.
			0.65f, 0.65f, 0.65f,   // 2.
			-0.65f, 0.65f, 0.65f,  // 3.

			//BACK
			-0.65f, -0.65f, -0.65f, // 4.
			0.65f, -0.65f, -0.65f,  // 5.
			0.65f, 0.65f, -0.65f,   // 6.
			-0.65f, 0.65f, -0.65f,  // 7.

			// TOP
			-0.65f, -0.65f, 0.65f,  // 8.
			-0.65f, -0.65f, -0.65f, // 9.
			0.65f, -0.65f, -0.65f,  // 10.
			0.65f, -0.65f, 0.65f,   // 11.

			//BOTTOM
			-0.65f, 0.65f, 0.65f,  // 12.
			-0.65f, 0.65f, -0.65f, // 13.
			0.65f, 0.65f, -0.65f,  // 14.
			0.65f, 0.65f, 0.65f,   // 15.

			// LEFT
			-0.65f, -0.65f, -0.65f, // 16.
			-0.65f, -0.65f, 0.65f,  // 17.
			-0.65f, 0.65f, 0.65f,   // 18.
			-0.65f, 0.65f, -0.65f,  // 19.

			//RIGHT
			0.65f, -0.65f, 0.65f,  // 20.
			0.65f, -0.65f, -0.65f, // 21.
			0.65f, 0.65f, -0.65f,  // 22.
			0.65f, 0.65f, 0.65f,   // 23.
		};

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLfloat texture_coords[] =
	{
		0, 0,
		0, 0.5f,
		0.5f, 0.5f,
		0.5f, 0,
		
		0, 0,
		0, 0.5f,
		0.5f, 0.5f,
		0.5f, 0,

		0, 0,
		0, 0.5f,
		0.5f, 0.5f,
		0.5f, 0,

		0, 0,
		0, 0.5f,
		0.5f, 0.5f,
		0.5f, 0,

		0, 0,
		0, 0.5f,
		0.5f, 0.5f,
		0.5f, 0,
				
				
		0, 0,
		0, 0.5f,
		0.5f, 0.5f,
		0.5f, 0,
	};

	texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLfloat plane_normals[] =
	{
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,

		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,

		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,

		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,

		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,

		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f,

	};

	normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	GLfloat colours[] = 
	{
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,

		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,

		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,

		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
	};
	
	colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.
	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.
}

//---------------------------------------------------------------------
//
// init
//
void init(void)
{
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	modelID = glGetUniformLocation(program, "model");
	projID = glGetUniformLocation(program, "projection");
	viewID = glGetUniformLocation(program, "view");
	
	resetView();


	// Load first image
	unsigned char*image = SOIL_load_image("Leather.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (!image) { cout << "Unable to load first file!" << endl; }

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	// Load second image
	image = SOIL_load_image("Fence.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (!image){cout << "Unable to load second file!" << endl;}

	glGenTextures(1, &textureID2);
	

	glBindTexture(GL_TEXTURE_2D, textureID2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);






	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	// Setting material values.
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);

	// Setting ambient Light.
	glUniform3f(glGetUniformLocation(program, "aLight.ambientColour"), aLight.ambientColour.x, aLight.ambientColour.y, aLight.ambientColour.z);
	glUniform1f(glGetUniformLocation(program, "aLight.ambientStrength"), aLight.ambientStrength);

	// Setting directional light.
	glUniform3f(glGetUniformLocation(program, "dLight.base.diffuseColour"), dLight.diffuseColour.x, dLight.diffuseColour.y, dLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "dLight.base.diffuseStrength"), dLight.diffuseStrength);

	glUniform3f(glGetUniformLocation(program, "dLight.direction"), dLight.direction.x, dLight.direction.y, dLight.direction.z);
	
	// Setting point light.
	glUniform3f(glGetUniformLocation(program, "pLight.base.diffuseColour"), pLight.diffuseColour.x, pLight.diffuseColour.y, pLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLight.base.diffuseStrength"), pLight.diffuseStrength);

	glUniform3f(glGetUniformLocation(program, "pLight.position"), pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight.constant"), pLight.constant);
	glUniform1f(glGetUniformLocation(program, "pLight.linear"), pLight.linear);
	glUniform1f(glGetUniformLocation(program, "pLight.exponent"), pLight.exponent);

	// Setting point light2.
	glUniform3f(glGetUniformLocation(program, "pLight2.base.diffuseColour"), pLight2.diffuseColour.x, pLight2.diffuseColour.y, pLight2.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLight2.base.diffuseStrength"), pLight2.diffuseStrength);

	glUniform3f(glGetUniformLocation(program, "pLight2.position"), pLight2.position.x, pLight2.position.y, pLight2.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight2.constant"), pLight2.constant);
	glUniform1f(glGetUniformLocation(program, "pLight2.linear"), pLight2.linear);
	glUniform1f(glGetUniformLocation(program, "pLight2.exponent"), pLight2.exponent);

	initSquare();
	

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);
	// Enable Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	// Enable face culling.
	// glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CW);
	// glCullFace(GL_BACK);

}

//---------------------------------------------------------------------
//
// calculateView
//
void calculateView()
{
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = glm::normalize(frontVec);
	rightVec = glm::normalize(glm::cross(frontVec, worldUp));
	upVec = glm::normalize(glm::cross(rightVec, frontVec));

	view = glm::lookAt(position, position + frontVec, upVec); 
	glUniform3f(glGetUniformLocation(program, "eyePosition"), position.x, position.y, position.z);
}

//---------------------------------------------------------------------
//
// transformObject
//
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation)
{
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(projID, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
}

//---------------------------------------------------------------------
//
// display
//
void display(void)
{
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the projection or view if perspective.
	projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	calculateView();

	// 1
	transformObject(3.0f, Y_AXIS, rotAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawElements(GL_QUADS, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	
	// 2
	transformObject(3.6f, Y_AXIS, rotAngle+=1, glm::vec3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glDrawElements(GL_QUADS, 36, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glutSwapBuffers(); // Now for a potentially smoother render.

}

void idle()
{
	//glutPostRedisplay();
}

void timer(int id)
{
	glutPostRedisplay();
	glutTimerFunc(33, timer, 0);
}

void keyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		position += frontVec * moveSpeed;
		break;
	case 's':
		position -= frontVec * moveSpeed;
		break;
	case 'a':
		position -= rightVec * moveSpeed;
		break;
	case 'd':
		position += rightVec * moveSpeed;
		break;
	case 'j':
		pLight.position.x -= 0.1f;
		pLight2.position.x -= 0.1f;

		break;
	case 'l':
		pLight.position.x += 0.1f;
		pLight2.position.x += 0.1f;

		break;
	case 'u':
		pLight.position.y += 0.1f;
		pLight2.position.y -= 0.1f;

		break;
	case 'o':
		pLight.position.y -= 0.1f;
		pLight2.position.y -= 0.1f;

		break;
	case 'i':
		pLight.position.z -= 0.1f;
		pLight2.position.z -= 0.1f;
		break;
	case 'k':
		pLight.position.z += 0.1f;
		pLight2.position.z += 0.1f;

		break;
	case ' ':
		resetView();
		break;
	}
	glUniform3f(glGetUniformLocation(program, "pLight.position"), pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform3f(glGetUniformLocation(program, "pLight2.position"), pLight2.position.x, pLight2.position.y, pLight2.position.z);
}

void keyDownSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		pitch -= turnSpeed;
		break;
	case GLUT_KEY_DOWN:
		pitch += turnSpeed;
		break;
	case GLUT_KEY_LEFT:
		yaw += turnSpeed;
		break;
	case GLUT_KEY_RIGHT:
		yaw -= turnSpeed;
		break;
	}
}

void mouseMove(int x, int y)
{
	if (mouseClicked)
	{
		pitch -= (GLfloat)((y - lastY) * 0.1);
		yaw += (GLfloat)((x - lastX) * 0.1);
		lastY = y;
		lastX = x;
	}
}

void mouseClick(int btn, int state, int x, int y)
{

	if (state == 0)
	{
		lastX = x;
		lastY = y;
		mouseClicked = true;
		glutSetCursor(GLUT_CURSOR_NONE);
		cout << "Mouse clicked." << endl;
	}
	else
	{
		mouseClicked = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);
		cout << "Mouse released." << endl;
	}
}

void clean()
{
	cout << "Cleaning up!" << endl;
	glDeleteTextures(1, &textureID);
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();

	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(keyDownSpecial);
	glutMouseFunc(mouseClick);

	glutMotionFunc(mouseMove); // Requires click to register.
	atexit(clean); // This GLUT function calls specified function before terminating program. Useful!
	glutMainLoop();
}
