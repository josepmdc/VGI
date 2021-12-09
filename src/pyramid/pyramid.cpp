// https://github.com/1kar/OpenGL-SolarSystem/blob/master/Sphere.h

#include <cstdlib>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#include "pyramid.h"
#include <gl/GL.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../util/util.h"
#include "../camera/camera.h"
#include <backends/imgui_impl_opengl3_loader.h>

GLuint p_VBO = 0;
void Pyramid::DrawingPyramid() {

    Camera pos_side;
    pos_side.getCameraUp();

    //Just have to fix position of the pyramid and call it in the main function

    std::vector<double> vertices = {

        2.5f,
        -2.5f,
        0.0f,
        -2.5f,
        2.5f,
        0.0f, //base triangle one
        -2.5f,
        -2.5f,
        0.0f,

        -2.5f,
        2.5f,
        0.0f,
        2.5f,
        2.5f,
        0.0f, //base triangle two
        2.5f,
        -2.5f,
        0.0f,

        2.5f,
        -2.5f,
        0.0f,
        2.5f,
        2.5f,
        0.0f, //triangle 1
        0.0f,
        0.0f,
        2.0f,

        2.5f,
        2.5f,
        0.0f,
        -2.5f,
        2.5f,
        0.0f, //triangle 2
        0.0f,
        0.0f,
        2.0f,

        -2.5f,
        2.5f,
        0.0f,
        -2.5f,
        -2.5f,
        0.0f, //triangle 3
        0.0f,
        0.0f,
        2.0f,

        -2.5f,
        -2.5f,
        0.0f,
        2.5f,
        -2.5f,
        0.0f, //triangle 4
        0.0f,
        0.0f,
        2.0f,
    };

    glGenBuffers(1, &p_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, p_VBO); // Activar l’objecte VBO
    // Reservar buffer per a l’objecte VBO
    glBufferData(GL_ARRAY_BUFFER, (vertices.size()) * sizeof(double), 0, GL_STATIC_DRAW);
    // Position Vertex atributes. Copy geometry data to VBO starting from 0 offset

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (GLvoid*)0); // Definir vèrtex en array VBO
    // Atribut de color dels vèrtexs. Copia del vector de color darrera el vector de vèrtexs

    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (GLvoid*)0); // Definir color en array VBO
    glDrawArrays(GL_TRIANGLES, 0, 36);                                                  // Dibuixar el cub
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                                   // Desactivar l’objecte VBO
    glDeleteBuffers(1, &p_VBO);
}