#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef MESH_RESOLUTION
#define MESH_RESOLUTION 80
#endif

/**
 * Generate vertices and populate the buffer arrays
 */
void makeGeometry(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);

/**
 * Generate vertices and populate the buffer arrays.
 * RES specifies resolution, NIDX is set to number of indices to be drawn.
 */
void makeGeometry(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO, 
                unsigned int n_verts, unsigned int n_idx, unsigned int res,
                unsigned int stride, float* vertices, unsigned int* indices);

#endif
