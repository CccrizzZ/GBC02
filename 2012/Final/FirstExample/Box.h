#pragma once
using namespace std;
#include <iostream>
#include "stdlib.h"
#include "GL\glew.h"
#include "time.h"
#include "vgl.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Box
{
public:
    float m_length;
    float m_rotationAng;

    glm::vec3 m_rotationAxis;
    glm::vec3 m_transition;
    glm::vec3 m_scale;


    GLuint ibo;
    GLuint points_vbo;
    GLuint colors_vbo;
    

public:
    Box::Box(){
        
    }

    // Constructor
    Box::Box(
        float len, 
        float angle,
        glm::vec3 scal,
        glm::vec3 rotAxis,
        glm::vec3 trans
    )
    {
        m_length = len;
        m_scale = scal;
        m_rotationAxis = rotAxis;
        m_rotationAng = angle;
        m_transition = trans;
    }


    // Destructor
    Box::~Box()
    {

    }


    // Init func
    void initBox()
    {

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
        GLfloat cube_vertices[24];
        for (int i = 0; i < 23; i++)
        {
            switch (i)
            {
            case 0:
            case 1:
            case 4:
            case 9:
            case 12:
            case 13:
            case 14:
            case 16:
            case 17:
            case 20:
            case 21:
            case 23:
                cube_vertices[i] = -m_length;
                break;
            default:
                cube_vertices[i] = m_length;
                break;
            }
        }
        
        
        srand(time(NULL));

        // Cube colors
        GLfloat cube_colors[24];
        for (int i = 0; i < 23; i+=3)
        {

            float temp1 = (static_cast<float>(rand()) / static_cast<float>(1))/100000.0f;
            float temp2 = (static_cast<float>(rand()) / static_cast<float>(1))/100000.0f;
            float temp3 = (static_cast<float>(rand()) / static_cast<float>(1))/100000.0f;

            cube_colors[i] = temp1;
            cube_colors[i + 1] = temp2;
            cube_colors[i + 2] = temp3;
        }

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
    }


    // display func
    void displayBox(GLuint Mid ,glm::mat4 m, glm::mat4 v, glm::mat4 p)
    {
        transformObject2(Mid,m,v,p,m_scale, m_rotationAxis, m_rotationAng, m_transition);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);
    }




    // utility
    void transformObject2(GLuint modelID, glm::mat4 mvp, glm::mat4 view, glm::mat4 projection, glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation)
    {
        glm::mat4 Model;
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, translation);
        Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
        Model = glm::scale(Model, scale);
        mvp = projection * view * Model;
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
    }
};
