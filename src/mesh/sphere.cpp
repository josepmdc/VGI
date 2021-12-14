// https://github.com/1kar/OpenGL-SolarSystem/blob/master/Sphere.h
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

#include "sphere.h"

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

Sphere::Sphere(float r, int sectors, int stacks) {
    m_Radius = r;
    m_SectorCount = sectors;
    m_StackCount = stacks;

    /* GENERATE VERTEX ARRAY */
    float x, y, z, xy;               // vertex position
    float lengthInv = 1.0f / m_Radius; // vertex normal
    float s, t;                      // vertex texCoord

    float sectorStep = (float)(2 * M_PI / m_SectorCount);
    float stackStep = (float)(M_PI / m_StackCount);
    float sectorAngle, stackAngle;

    for (int i = 0; i <= m_StackCount; ++i) {
        stackAngle = (float)(M_PI / 2 - i * stackStep); // starting from pi/2 to -pi/2
        xy = 1.02f * m_Radius * cosf(stackAngle);         // r * cos(u)
        y = m_Radius * sinf(stackAngle);                  // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different
        // tex coords
        for (int j = 0; j <= m_SectorCount; ++j) {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            z = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            m_Vertices.push_back(x);
            m_Vertices.push_back(y);
            m_Vertices.push_back(z);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / m_SectorCount;
            t = (float)i / m_StackCount;
            m_Vertices.push_back(s);
            m_Vertices.push_back(t);
        }
    }
    /* GENERATE VERTEX ARRAY */

    /* GENERATE INDEX ARRAY */
    int k1, k2;
    for (int i = 0; i < m_StackCount; ++i) {
        k1 = i * (m_SectorCount + 1); // beginning of current stack
        k2 = k1 + m_SectorCount + 1;  // beginning of next stack

        for (int j = 0; j < m_SectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                m_Indices.push_back(k1);
                m_Indices.push_back(k2);
                m_Indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (m_StackCount - 1)) {
                m_Indices.push_back(k1 + 1);
                m_Indices.push_back(k2);
                m_Indices.push_back(k2 + 1);
            }
        }
    }
    /* GENERATE INDEX ARRAY */

    /* GENERATE VAO-EBO */
    // GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
    // attribute pointer(s).
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        (unsigned int)m_Vertices.size() * sizeof(float),
        m_Vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        (unsigned int)m_Indices.size() * sizeof(unsigned int),
        m_Indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::Draw() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)m_Indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
