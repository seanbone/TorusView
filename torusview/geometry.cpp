#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "geometry.h"

#include <iostream>
#include <cmath>

void makeGeometry(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO) {

    //- Vertex coordinates
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    //- Vertex Buffer Object and Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    //- Link vertex attributes
    // Vertex coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
                          (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void torusTransform(float alpha, float beta, glm::vec3& pos, glm::vec3& norm) {
    using namespace glm;

    pos = vec3(0.0f);
    norm = vec3(0.0f);

    float R = 1.0f;
    float r = 0.4f;

    pos.x =  R*cos(2*M_PI*alpha);
    pos.z = -R*sin(2*M_PI*alpha);

    vec3 dir1 = pos / length(pos);
    vec3 dir2 = dir1 * (float)cos(2*M_PI*beta);
    dir2.y = sin(2*M_PI*beta);
    dir2 /= length(dir2);

    pos -= dir2 * r;
    //pos.y = beta;
    norm = dir2;
}

void makeGeometry(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO,
        unsigned int n_verts, unsigned int n_idx, unsigned int res,
        unsigned int stride, float* out_verts, unsigned int* out_indices) {

    //- Vertex coordinates
    //unsigned int stride = 5; 
    float vertices[n_verts * stride];
    float xmin =  0.0f;
    float xmax =  1.0f;
    float ymin =  0.0f;
    float ymax =  1.0f;

    float dx = (xmax - xmin) / (res - 1.);
    float dy = (ymax - ymin) / (res - 1.);

    for (unsigned int i = 0; i < n_verts; i++) {
        float x = dx * (i % res);
        float y = dy * (i / res);
        glm::vec3 pos, norm;
        torusTransform(xmin + x, ymin + y, pos, norm);
        //vertices[stride*i + 0] = xmin+x;
        //vertices[stride*i + 1] = ymin+y;
        //vertices[stride*i + 2] = 1.0f  ;
        vertices[stride*i + 0] = pos.x;
        vertices[stride*i + 1] = pos.y;
        vertices[stride*i + 2] = pos.z;
        vertices[stride*i + 3] = x / (xmax - xmin);
        vertices[stride*i + 4] = y / (ymax - ymin);
        vertices[stride*i + 5] = norm.x;
        vertices[stride*i + 6] = norm.y;
        vertices[stride*i + 7] = norm.z;
    }
    // For whatever reason we can't use the same array, we need to copy it out
    //  or it won't render.
    std::copy(vertices, vertices + n_verts*stride, out_verts);

    //- Triangle indices
    unsigned int indices[n_idx];

    unsigned int cnt = 0;
    for (unsigned int iy = 0; iy < res-1; iy++) {
        for (unsigned int ix = 0; ix < res-1; ix++) {
            unsigned int i = iy * res + ix;
            indices[6*cnt + 0] = i;
            indices[6*cnt + 1] = i+1;
            indices[6*cnt + 2] = i+res+1;
            indices[6*cnt + 3] = i;
            indices[6*cnt + 4] = i+res+1;
            indices[6*cnt + 5] = i+res;
            cnt++;
        }
    }
    std::copy(indices, indices+n_idx, out_indices);

    //- Vertex Buffer Object and Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    //- Link vertex attributes
    // Vertex coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride*sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride*sizeof(float),
                          (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal vector
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride*sizeof(float),
                          (void*) (5*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
