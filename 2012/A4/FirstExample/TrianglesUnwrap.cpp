
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
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
GLuint textureID;
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
				    0.05f);							// Ambient strength.

DirectionalLight dLight(glm::vec3(1.0f, 1.0f, 0.0f), // Direction.
						glm::vec3(1.0f, 1.0f, 0.0f),  // Diffuse colour.
						0.0f);						  // Diffuse strength.

PointLight pLight(glm::vec3(5.0f, 0.0f, 0.75f),	// Position.
				  1.0f, 0.7f, 1.8f,				// Constant, Linear, Exponent.
				  glm::vec3(0.0f, 2.0f, 1.0f),	// Diffuse colour.
				  2.0f);						// Diffuse strength.

SpotLight sLight(glm::vec3(0.0f, -0.75f, 1.0f),	// Position.
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
	position = glm::vec3(5.0f, 5.0f, 10.0f);
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0, 1, 0);
	pitch = -15.0f;
	yaw = -90.0f;
}

//---------------------------------------------------------------------
//
// init
//
void initPlane(void)
{
	int i;
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Indices.
	GLushort indices[600];
	i = 0;
	for (int r = 0; r < 10; r++)
	{
		for (int c = 0; c < 10; c++)
		{
			indices[i] = (c + r * 11);
			indices[i + 1] = (c + (r + 1) * 11);
			indices[i + 2] = ((c + 1) + (r + 1) * 11);
			indices[i + 3] = ((c + 1) + (r + 1) * 11);
			indices[i + 4] = ((c + 1) + r * 11);
			indices[i + 5] = (c + r * 11);
			i += 6;
		}
	}
	ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertices.
	GLfloat vertices[363];
	i = 0;
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			vertices[i] = c * 0.2f;
			vertices[i + 1] = r * 0.15f;
			vertices[i + 2] = 0.0f;
			i += 3;
		}
	}
	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// UVs
	GLfloat texture_coords[242];
	i = 0;
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			texture_coords[i] = c * 0.1f;
			texture_coords[i + 1] = r * 0.1f;
			i += 2;
		}
	}
	texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	   	
	// Normals.
	GLfloat plane_normals[363];
	i = 0;
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			plane_normals[i] = 0.0f;
			plane_normals[i + 1] = 0.0f;
			plane_normals[i + 2] = -1.0f;
			i += 3;
		}
	}
	normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Colours. I can still tint the fragments if I wanted to.
	GLfloat colours[363]; 
	i = 0;
	for (int r = 0; r < 11; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			colours[i] = 1.0f;
			colours[i + 1] = 1.0f;
			colours[i + 2] = 1.0f;
			i += 3;
		}
	}
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
// initSquare
//
void initSquare(void)
{
	int i;
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLushort indices[] = {
		0, 1, 2, 
		2, 3, 0 };
	ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f };
	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLfloat texture_coords[] = {
	0,0,
	0,1, // Change to 1,0 for nightmare fuel.
	1,1,
	1,0 };
	texture_vbo = 0;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLfloat plane_normals[] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f };
	normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	GLfloat colours[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f };
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

	// Loading first image.
	//stbi_set_flip_vertically_on_load(true);
	//unsigned char* image = stbi_load("d20.png", &width, &height, &bitDepth, 0);
	unsigned char*image = SOIL_load_image("Floor.jpg", &width, &height, 0, SOIL_LOAD_RGB);
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
	//stbi_image_free(image);
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

 	//initSquare();
	initPlane();

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);
	// Enable face culling.
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
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

	//transformObject(1.5f, X_AXIS, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(7.0f, X_AXIS, -90.0f, glm::vec3(-1.5f, -1.0f, 0.0f));
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glDrawElements(GL_TRIANGLES, 600, GL_UNSIGNED_SHORT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	// Done.

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
		break;
	case 'l':
		pLight.position.x += 0.1f;
		break;
	case 'u':
		pLight.position.y += 0.1f;
		break;
	case 'o':
		pLight.position.y -= 0.1f;
		break;
	case 'i':
		pLight.position.z -= 0.1f;
		break;
	case 'k':
		pLight.position.z += 0.1f;
		break;
	case ' ':
		resetView();
		break;
	}
	glUniform3f(glGetUniformLocation(program, "pLight.position"), pLight.position.x, pLight.position.y, pLight.position.z);
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
	//cout << "Mouse pos: " << x << "," << y << endl;
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
	/*cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;*/
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
	//glutPassiveMotionFunc(mouseMove); // or...
	glutMotionFunc(mouseMove); // Requires click to register.
	atexit(clean); // This GLUT function calls specified function before terminating program. Useful!
	glutMainLoop();
}
